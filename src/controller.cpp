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
    currentIntensity = 0;
    highestIntensity = 0;

    // Initialize context
    this->context["sessionSelection"]    = false;
    this->context["connectionTest"]      = false;
    this->context["activeSession"]       = false;
    this->context["pausedSession"]       = false;
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
    if (getContext("sessionSelection"))
    {
        currentSession = new Session(true, 0.5, 20, SessionType::SUB_DELTA); // HARDCODED SELECTED SESSION
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

        emit sessionProgress(currentSession->getPresetDurationSeconds(), currentSession->getType());
    }
    else if (getContext("promptRecordSession"))
    {
        stopRecordPrompt(true);
    }
}

void Controller::handleDownClicked()
{

    if (getContext("activeSession"))
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
    if (getContext("activeSession"))
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
        emit sessionProgress(remainingSeconds, sessionType);

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
        shutDownTimer->start(IDLE_TIMEOUT_MS);
    }

}

void Controller::stopSession()
{
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
        emit useSelectionContext();
    }

}

void Controller::handlePowerClicked()
{
    if (getContext("activeSession"))
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

    if (isPowerOn)
    {
        //Turn on device
        emit powerOn();
        shutDownTimer->start(IDLE_TIMEOUT_MS);
        //FOR TESTING
        setContext("sessionSelection");
        //TODO DISPLAY BATTERY LEVEL
    }
    else
    {
        //Turn off device
        emit powerOff();
        shutDownTimer->stop();
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
    connect(earClips, SIGNAL(connectionLevel(int, QString)), this, SLOT(handleEarClipConnectionLevel(int, QString)));
}

void Controller::changeBattery(Battery *b)
{
    delete currentBattery;
    currentBattery = b;
}

void Controller::handleEarClipConnectionLevel(int level, QString disconnected)
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
        sendEarClipConnection(level, "None");

    }
    else if (getContext("activeSession") && level == 0)
    {
        // blink no connection (number 7 and 8) on CES
        // could send a different signal to be recieved by MainWinow
        // that will blink the numbers and the R or L depending on
        // which earclip was disconnected.
        // connectionModeLight(currentSession->isShortPulse());
        connectionModeLight(true); // for testing purposes
        sendEarClipConnection(level, disconnected);
        //stopSession();
        setContext("pausedSession");
        pausedSession();
    }
    else if(getContext("pausedSession"))
    {
        sendEarClipConnection(level, disconnected);
    }


}

void Controller::handleEarClipConnection(int index)
{
    earClipsAreConnected = index;
    earClips->earClipConnectionTest();
}

void Controller::pausedSession()
{
    if (getContext("pausedSession"))
    {
        int temp = earClips->earClipConnectionTest();
        for (int i = 0; i < 10; i++)
        {
            if (temp > 0 && earClipsAreConnected)
            {
                setContext("activeSession");
                for (int j = currentIntensity; j > 0; j--)
                {
                    handleDownClicked();
                    delayMs(200);
                }
                // uncomment once merged with main branch
                // resetShutDownTimer();
                return;
            }
            delayMs(500);
            temp = earClips->earClipConnectionTest();
        }
        stopSession();
    }
}
