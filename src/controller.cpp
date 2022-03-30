#include "controller.h"
#include <QDebug>
#include <QtGlobal>

Controller::Controller(Battery *b, QList<Group *> groups, QObject *parent) : QObject(parent)
{
    earClips       = nullptr;
    currentBattery = b;
    isPowerOn      = false;
    this->groups   = groups;
    currentSession = nullptr;
    remainingSessionTime = nullptr;

    // Initialize context
    this->context["sessionSelection"] = false;
    this->context["connectionTest"] = false;
    this->context["session"] = false;
    this->context["promptRecordSession"] = false;

    // Timers
    timerId = startTimer(1000);
    shutDownTimer =  new QTimer(this);
    connect(shutDownTimer, &QTimer::timeout, [this]() { emit powerOff(); });
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
    //If context is SESSION
    if (getContext("session"))
    {
        currentSession = new Session(true, 0.5, 10, SessionType::SUB_DELTA); // HARDCODED SELECTED SESSION
        //Set timer to session duration
        remainingSessionTime = new QTimer(this);
        //When timer reaches 0
        connect(remainingSessionTime, &QTimer::timeout, [this]() { stopSession(); });
        remainingSessionTime->start(currentSession->getPresetDurationSeconds()*1000);

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
    // Emits progress of active session
    if (getContext("session") && currentSession != nullptr)
    {
        int runningSeconds = remainingSessionTime->remainingTime() / 1000;
        SessionType sessionType = currentSession->getType();
        emit sessionProgress(runningSeconds, sessionType);

        // Constantly refresh shut down timer during active session
        shutDownTimer->start(50000);
    }

}

void Controller::stopSession()
{
    elapsedSessionTime = currentSession->getPresetDurationSeconds() - (remainingSessionTime->remainingTime() / 1000);
    //Stop timer
    remainingSessionTime->stop();

    setContext("promptRecordSession");    
    emit sessionEnds();
}

void Controller::stopRecordPrompt(bool shouldRecord)
{
    if (shouldRecord)
    {
        Record* record = new Record(elapsedSessionTime, 5, currentSession->getType());
        remainingSessionTime->stop();
        history.append(record);
        emit newRecord(record);
    }

    delete currentSession;
    delete remainingSessionTime;
    currentSession = nullptr;
    remainingSessionTime = nullptr;

    //Set next context
    setContext("sessionSelection");
    emit useSelectionContext();
}

void Controller::handlePowerClicked()
{
    if (getContext("session"))
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

        if (isPowerOn)
        {
            //Turn on device
            emit powerOn();
            shutDownTimer->start(50000);
            //FOR TESTING
            setContext("session");
        }
        else
        {
            //Turn off device
            emit powerOff();
            shutDownTimer->stop();
        }
    }
}

void Controller::togglePower(){
    isPowerOn = !isPowerOn;
}

void Controller::setEarClips(EarClips *e)
{
    if (earClips != nullptr)
    {
        delete earClips;
    }
    earClips = e;
}

void Controller::changeBattery(Battery *b)
{
    delete currentBattery;
    currentBattery = b;
}
