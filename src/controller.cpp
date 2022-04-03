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
    if (getContext("sessionSelection"))
    {
        currentSession = new Session(true, 0.5, 10, SessionType::SUB_DELTA); // HARDCODED SELECTED SESSION
        elapsedSessionTime = 0;

        setContext("activeSession");  // TODO connection test

        emit sessionProgress(currentSession->getPresetDurationSeconds(), currentSession->getType());
    }
    else if (getContext("promptRecordSession"))
    {
        stopRecordPrompt(true);
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
        Record* record = new Record(elapsedSessionTime, 5, currentSession->getType());
        history.append(record);
        emit newRecord(record);
    }

    delete currentSession;
    currentSession = nullptr;

    //Set next context
    setContext("sessionSelection");
    emit useSelectionContext();
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
    connect(earClips, SIGNAL(connectionLevel(int)), this, SLOT(handleEarClipConnectionLevel(int)));
}

void Controller::changeBattery(Battery *b)
{
    delete currentBattery;
    currentBattery = b;
}

void Controller::handleEarClipConnectionLevel(int level)
{

    if (earClips == nullptr)
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
        //in this case the session must be stopped.
    }

}

