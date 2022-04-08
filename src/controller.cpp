#include "controller.h"
#include <QDebug>
#include <QtGlobal>

int Controller::IDLE_TIMEOUT_MS = 30000;

Controller::Controller(Battery *b, QList<Group *> groups, EarClips *earClips, QObject *parent) : QObject(parent)
{
    this->earClips          = earClips;
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
    delete currentBattery;
    delete earClips;
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
        if (groups[selectedGroupIndex]->size() == 0)
        {
            return;
        }

        currentSession = groups[selectedGroupIndex]->getSession(selectedSessionIndex);
        //All sessions by default will be at intensity level 1
        currentIntensity = 1;
        highestIntensity = 1;
        emit adjustSessionIntensity(currentIntensity);

        elapsedSessionTime = 0;

        connectionTest();

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
        if (groups[selectedGroupIndex]->size() > 0)
        {
            selectedSessionIndex = ((selectedSessionIndex - 1) + groups[selectedGroupIndex]->size()) % groups[selectedGroupIndex]->size();
            emit selectSession(selectedSessionIndex, groups[selectedGroupIndex]->getSession(selectedSessionIndex));
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
        if (groups[selectedGroupIndex]->size() > 0)
        {
            selectedSessionIndex = (selectedSessionIndex + 1) % groups[selectedGroupIndex]->size();
            emit selectSession(selectedSessionIndex, groups[selectedGroupIndex]->getSession(selectedSessionIndex));
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

        //Battery depletes every second scaled by intensity level and ear clip connection level
        currentBattery->deplete(((currentIntensity + 1)/2) + earClips->minConnectionLevel());

        emit sessionProgress(remainingSeconds, sessionType, currentBattery->getBatteryLevel());

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

    currentSession = nullptr;

    if(currentBattery->isCriticallyLow())
    {
        emit batteryShutDown();
        togglePower();
    }
    else
    {
        //Set next context
        setContext("sessionSelection");
        emit useSelectionContext();
        selectedGroupIndex   = 0;
        selectedSessionIndex = 0;
        emit selectGroup(groups[selectedGroupIndex]);
        emit selectSession(selectedSessionIndex, groups[selectedGroupIndex]->getSession(selectedSessionIndex));
    }
}

void Controller::handlePowerClicked()
{
    // If timer is not active, the user has hold power button for 2+ seconds. So, we ignore the clicked signal
    if (!powerPressedTimer->isActive())
    {
        return;
    }

    powerPressedTimer->stop();
    resetShutDownTimer();

    if (getContext("sessionSelection"))
    {
        selectedGroupIndex   = (selectedGroupIndex + 1) % groups.size();
        emit selectGroup(groups[selectedGroupIndex]);

        //Prevent showing inexistent session option by setting index outside 0-3 range (nothing lights up)
        selectedSessionIndex = groups[selectedGroupIndex]->size() == 0 ? -1 : 0;
        Session *selectedSession = selectedSessionIndex == -1 ? nullptr : groups[selectedGroupIndex]->getSession(selectedSessionIndex);
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
}

void Controller::handlePowerPressed()
{
    // Toggle power once 2 seconds have passed, and stop timer to prevent it from repeating
    powerPressedTimer = new QTimer();
    connect(powerPressedTimer, &QTimer::timeout, [this](){ powerPressedTimer->stop(); togglePower(); });
    powerPressedTimer->start(2000);
}

void Controller::togglePower(){
    isPowerOn = !isPowerOn;
    qDebug() << "Power on:" << isPowerOn;

    if (isPowerOn)
    {
        //Turn on device

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
        Session *selectedSession = groups[selectedGroupIndex]->size() == 0 ? nullptr : groups[selectedGroupIndex]->getSession(selectedSessionIndex);
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

void Controller::changeBattery(Battery *b)
{
    delete currentBattery;
    currentBattery = b;
}

void Controller::connectionTest()
{
    setContext("connectionTest");

    int connectionTestValue = earClipsAreConnected ? earClips->minConnectionLevel() : 0;
    // Ends either through ear clips being fixed or shutting down through timeout
    while (connectionTestValue <= 0)
    {
        if (!shutDownTimer->isActive())
        {
            return;
        }

        if (earClipsAreConnected)
        {
            emit sendEarClipConnection(connectionTestValue, earClips->isLeftConnected(), earClips->isRightConnected());
        }
        else
        {
            // Send true meaning earClips are connected (even though we don't know if they are)
            // so that the lights don't flash
            emit sendEarClipConnection(connectionTestValue, true, true);
        }
        connectionTestValue = earClipsAreConnected ? earClips->minConnectionLevel() : 0;
    }

    emit sendEarClipConnection(connectionTestValue, true, true);

    setContext("activeSession");
}

void Controller::handleEarClipsChanged(int level)
{
    if (getContext("activeSession"))
    {
        int connectionTestValue = earClipsAreConnected ? earClips->minConnectionLevel() : 0;
        if (connectionTestValue <= 0)
        {
            pauseSession();
        }
    }
}

void Controller::handleEarClipsPluggedIn(int selectionIndex)
{
    // selectionIndex 0 means disconnected
    earClipsAreConnected = selectionIndex;

    handleEarClipsChanged(0);
}

void Controller::pauseSession()
{
    qDebug() << "paused";

    connectionTest();

    for (int i = currentIntensity; i > 0; i--)
    {
        handleDownClicked();
        delayMs(400);
    }
    resetShutDownTimer();

    qDebug() << "resumed";
}

void Controller::resetShutDownTimer()
{
    if (isPowerOn)
    {
        shutDownTimer->start(IDLE_TIMEOUT_MS);
    }
}
