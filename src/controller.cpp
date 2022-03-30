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
    this->context["navigatingHistory"] = false;


    // Timers
    startTimer(1000);
    shutDownTimer =  new QTimer(this);
    connect(shutDownTimer, &QTimer::timeout, [this]() { emit powerOff(); });

}

Controller::~Controller()
{
    //killTimer();
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
    if(getContext("session"))
    {
        currentSession = new Session(true, 0.5, 45, SessionType::SUB_DELTA); // HARDCODED SELECTED SESSION
        //Set timer to session duration
        remainingSessionTime = new QTimer(this);
        connect(remainingSessionTime, &QTimer::timeout, [this]() { recordSession(currentSession); });
        remainingSessionTime->start(currentSession->getPresetDurationSeconds()*1000);

    }else if(getContext("promptRecordSession")){
        //User wants to record the current session
        recordSession(currentSession);
        delete currentSession;
        delete remainingSessionTime;
        currentSession = nullptr;
        remainingSessionTime = nullptr;

        //Set next context
        setContext("sessionSelection");
    }

}

void Controller::timerEvent(QTimerEvent *event)
{
    qDebug() << "Timer Event\n";
    //If context is SESSION and currentSession is not null
    if(getContext("session") && currentSession != nullptr)
    {
        int runningSeconds = remainingSessionTime->remainingTime() / 1000;
        SessionType sessionType = currentSession->getType();
        emit sessionProgress(runningSeconds, sessionType);
    }

}

void Controller::stopSession(){

    //Stop timer
    remainingSessionTime->stop();

    setContext("promptRecordSession");
    emit sessionEnds();

    //Save session data

   //Set current session to null
}

//REVIEW
void Controller::handlePowerClicked()
{
    if(getContext("session"))
    {
        //The session stops

    }else if(getContext("promptRecordSession")){
        delete currentSession;
        delete remainingSessionTime;
        currentSession = nullptr;
        remainingSessionTime = nullptr;

        //Set next context
        setContext("sessionSelection");
    }

    togglePower();

    if(isPowerOn){
        //Turn on device
        emit powerOn();
        shutDownTimer->start(5000);
    }else{
        //Turn off device
        emit powerOff();
        shutDownTimer->stop();
    }
}

Record* Controller::recordSession(Session *session)
{
    //Get time spent for a session
    int sessionTime = session->getPresetDurationSeconds() - (remainingSessionTime->remainingTime() / 1000 );
    // TODO use actual Session
    Record* record = new Record(sessionTime, 5, session->getType());
    remainingSessionTime->stop();
    history.append(record);
    emit newRecord(record);
    return record;
}


//Reset timer
void Controller::resetTimeout(int ms)
{
    //Resets timer to given time
    shutDownTimer->start(ms);
}

bool Controller::getPowerStatus(){
    return isPowerOn;
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
