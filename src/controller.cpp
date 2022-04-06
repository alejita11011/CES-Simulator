#include "controller.h"
#include <QDebug>
#include <QtGlobal>

int Controller::IDLE_TIMEOUT_MS = 30000;

Controller::Controller(Battery *b, QList<Group *> groups, QObject *parent) : QObject(parent)
{
    earClips                = nullptr;
    currentBattery          = b;
    isPowerOn               = false;
    this->groups            = groups;
    currentSession          = nullptr;
    elapsedSessionTime      = 0;
    earClipsAreConnected    = false;
    currentIntensity        = 0;
    highestIntensity        = 0;
    selectedGroupIndex      = 0;
    selectedSessionIndex    = 0;

    // Initialize context
    this->context["sessionSelection"]    = false;
    this->context["connectionTest"]      = false;
    this->context["activeSession"]       = false;
    this->context["promptRecordSession"] = false;

    // Timers
    timerId = startTimer(1000);
    shutDownTimer = new QTimer(this);  // Will be started when device is turned on
    connect(shutDownTimer, &QTimer::timeout, [this](){ togglePower(); });
}

Controller::~Controller()
{
    killTimer(timerId);
    for (Record* record : history)
    {
        delete record;
    }

    for (Group* group: groups)
    {
        delete group;
    }
}

bool Controller::getContext(QString context)
{
    return this->context[context];
}

bool Controller::setContext(QString context)
{
    QList<QString> contexts = this->context.keys();

    // Check that the context exists
    if (!contexts.contains(context))
    {
        return false;
    }

    // Remove false value
    for (const QString &currentContext : qAsConst(contexts))
    {
        // There should only be one true value at any given time.
        // So, we can stop once we find it.
        if (this->context[currentContext] == true)
        {
            this->context[currentContext] = false;
            break;
        }
    }

    this->context[context] = true;

    return true;
}

void Controller::resetContext()
{
     QList<QString> contexts = this->context.keys();

     for (const QString &context : contexts)
     {
         this->context[context] = false;
     }
}

//Start selected session
void Controller::handleSelectClicked()
{
    resetShutDownTimer();

    if (getContext("sessionSelection"))
    {
        if (groups[selectedGroupIndex]->getSessions().size() == 0)
        {
            return;
        }

        currentSession = groups[selectedGroupIndex]->getSessions()[selectedSessionIndex];
        //All sessions by default will be at intensity level 1
        currentIntensity = 1;
        highestIntensity = 1;
        emit adjustSessionIntensity(currentIntensity);

        elapsedSessionTime = 0;

        setContext("connectionTest");
        int temp = earClips->earClipConnectionTest();
        while (temp <= 0 || !earClipsAreConnected)
        {
            delayMs(500);
            temp = earClips->earClipConnectionTest();
        }
        setContext("activeSession");

        emit sessionProgress(currentSession->getPresetDurationSeconds(), currentSession->getType(), currentBattery->getBatteryLevel());
    }
    else if (getContext("promptRecordSession"))
    {
        stopRecordPrompt(true);
    }
}

void Controller::handleDownClicked()
{
    resetShutDownTimer();

    if (getContext("sessionSelection"))
    {
        qDebug() << selectedSessionIndex;
        if (groups[selectedGroupIndex]->getSessions().size() > 0)
        {
            selectedSessionIndex = ((selectedSessionIndex - 1) + groups[selectedGroupIndex]->getSessions().size()) % groups[selectedGroupIndex]->getSessions().size();
            emit selectSession(selectedSessionIndex, groups[selectedGroupIndex]->getSessions()[selectedSessionIndex]);
        }
        else
        {
            emit selectSession(selectedSessionIndex, nullptr);
        }
    }
    else if (getContext("activeSession"))
    {

        if (currentIntensity > 1)
        {
            currentIntensity--;
            emit adjustSessionIntensity(currentIntensity);
        }

    }
}

void Controller::handleUpClicked()
{
    resetShutDownTimer();

    if (getContext("sessionSelection"))
    {
        if (groups[selectedGroupIndex]->getSessions().size() > 0)
        {
            selectedSessionIndex = (selectedSessionIndex + 1) % groups[selectedGroupIndex]->getSessions().size();
            emit selectSession(selectedSessionIndex, groups[selectedGroupIndex]->getSessions()[selectedSessionIndex]);
        }
        else
        {
            emit selectSession(selectedSessionIndex, nullptr);
        }
    }
    else if (getContext("activeSession"))
    {
        if (currentIntensity < 8)
        {
            currentIntensity++;
            highestIntensity = std::max(currentIntensity, highestIntensity);
            emit adjustSessionIntensity(currentIntensity);
        }

    }
}

// Triggered once every second
void Controller::timerEvent(QTimerEvent *event)
{
    if (getContext("activeSession"))
    {
        // Increment elapsed time
        elapsedSessionTime++;
        // Emit progress of active session
        int remainingSeconds = currentSession->getPresetDurationSeconds() - elapsedSessionTime;
        SessionType sessionType = currentSession->getType();
        emit sessionProgress(remainingSeconds, sessionType, currentBattery->getBatteryLevel());

        //Battery depletes every second scaled by intensity level
        currentBattery->deplete((currentIntensity + 1)/2);

        qDebug() << currentBattery->getBatteryLevel(); // FOR TESTING

        if (currentBattery->isCriticallyLow())
        {
            //Prevents multiple blinks
            setContext("promptRecordSession");
            for (int i = 0; i < 3; i++)
            {
                emit batteryLevel(true);
                delayMs(1000);
            }
            stopSession();

        }else if (currentBattery->isLow()){

            emit batteryLevel(false);
        }

        if (remainingSeconds == 0)
        {
            stopSession();
        }

        // Constantly refresh shut down timer during active session
        resetShutDownTimer();
    }

}

void Controller::stopSession()
{
    qDebug() << "Session stopping";
    setContext("promptRecordSession");
    emit sessionEnds();
}

void Controller::stopRecordPrompt(bool shouldRecord)
{
    if (shouldRecord)
    {
        Record* record = new Record(elapsedSessionTime, highestIntensity, currentSession->getType());
        history.append(record);
        emit newRecord(record);
    }

    delete currentSession;
    currentSession = nullptr;

    if(currentBattery->isCriticallyLow())
    {
        emit batteryShutDown();
        togglePower();
    }else{
        //Set next context
        setContext("sessionSelection");
        qDebug() << "HERE IN USESELECTION"; // FOR TESTING
        emit useSelectionContext();
        selectedGroupIndex   = 0;
        selectedSessionIndex = 0;
        emit selectGroup(groups[selectedGroupIndex]);
        emit selectSession(selectedSessionIndex, groups[selectedGroupIndex]->getSessions()[selectedSessionIndex]);
    }

}

void Controller::handlePowerClicked()
{
    resetShutDownTimer();

    if (getContext("sessionSelection"))
    {
        selectedGroupIndex   = (selectedGroupIndex + 1) % groups.size();
        emit selectGroup(groups[selectedGroupIndex]);

        //Prevent showing inexistent session option by setting index outside 0-3 range (nothing lights up)
        selectedSessionIndex = groups[selectedGroupIndex]->getSessions().size() == 0 ? -1 : 0;
        Session *selectedSession = selectedSessionIndex == -1 ? nullptr : groups[selectedGroupIndex]->getSessions()[selectedSessionIndex];
        emit selectSession(selectedSessionIndex, selectedSession);
    }
    else if (getContext("activeSession"))
    {
        //The session stops
        stopSession();
    }
    else if (getContext("promptRecordSession"))
    {
        stopRecordPrompt(false);
    }
    else
    {
        togglePower();
    }
}

void Controller::togglePower(){
    isPowerOn = !isPowerOn;
    qDebug() << "Power on:" << isPowerOn;

    if (isPowerOn)
    {
        //Turn on device
        qDebug() << currentBattery->getBatteryLevel(); // FOR TESTING

        if (currentBattery->isCriticallyLow())
        {
            //Turn off device
            emit batteryShutDown();
            togglePower();
            return;
        }

        emit powerOn(currentBattery->getBatteryLevel(), currentBattery->isLow());
        resetShutDownTimer();
        setContext("sessionSelection");

        // Update UI
        emit selectGroup(groups[selectedGroupIndex]);
        Session *selectedSession = groups[selectedGroupIndex]->getSessions().size() == 0 ? nullptr : groups[selectedGroupIndex]->getSessions()[selectedSessionIndex];
        emit selectSession(selectedSessionIndex, selectedSession);
    }
    else
    {
        //Turn off device
        emit powerOff();
        shutDownTimer->stop();
        resetContext();
    }
}

void Controller::setEarClips(EarClips *e)
{
    if (earClips != nullptr)
    {
        delete earClips;
    }
    earClips = e;
    // handle connectionLevel signal from EarClips
    connect(earClips, SIGNAL(connectionLevel(int)), this, SLOT(handleEarClipConnectionLevel(int)));
}

void Controller::changeBattery(Battery *b)
{
    delete currentBattery;
    currentBattery = b;
}

void Controller::handleEarClipConnectionLevel(int level)
{

    if (!earClipsAreConnected)
    {
        level = 0;
    }
    if (getContext("connectionTest"))
    {
        // the line below is the line we want to run but for now it
        // crashes the app since we have no currentSession
        // connectionModeLight(currentSession->isShortPulse());
        connectionModeLight(true); // for testing purposes
        sendEarClipConnection(level);

    }
    else if (getContext("activeSession") && level == 0)
    {
        // connectionModeLight(currentSession->isShortPulse());
        connectionModeLight(true); // for testing purposes
        sendEarClipConnection(level);
        stopSession();
    }


}

void Controller::handleEarClipConnection(int index)
{
    earClipsAreConnected = index;
    earClips->earClipConnectionTest();
}

void Controller::resetShutDownTimer()
{
    if (isPowerOn)
    {
        shutDownTimer->start(IDLE_TIMEOUT_MS);
    }
}
