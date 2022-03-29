#include "controller.h"
#include <QDebug>

Controller::Controller(Battery *b, QList<Group *> groups, QObject *parent) : QObject(parent)
{
    earClips       = nullptr;
    currentBattery = b;
    isPowerOn      = false;
    this->groups   = groups;
    currentSession = nullptr;
    remainingSessionTime = nullptr;
    qDebug() << "Controller constructor\n";
    startTimer(1000);
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

//Start selected session
void Controller::handleSelectClicked()
{
    //If context is SESSION
    currentSession = new Session(true, 0.5, 45, SessionType::SUB_DELTA); // HARDCODED SELECTED SESSION
    //Set timer to session duration
    remainingSessionTime = new QTimer(this);
    connect(remainingSessionTime, &QTimer::timeout, [this]() { recordSession(currentSession); });
    remainingSessionTime->start(currentSession->getPresetDurationSeconds()*1000);


}

void Controller::timerEvent(QTimerEvent *event)
{
    qDebug() << "Timer Event\n";
    //If context is SESSION and currentSESSION is not null
    if(currentSession != nullptr)
    {
        int runningSeconds = remainingSessionTime->remainingTime() / 1000;
        SessionType sessionType = currentSession->getType();
        emit sessionProgress(runningSeconds, sessionType);
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

//Initialize timer
void Controller::initializeTimer(QListWidget *display)
{
    mTimer = new QTimer(this);
    // connect(mTimer, SIGNAL(timeout()), this, SLOT(deviceShutDown(display)));
    connect(mTimer, &QTimer::timeout, [this, display]() { deviceShutDown(display); });
    mTimer->start(50000);
}

//Reset timer
void Controller::resetTimeout(int ms)
{
    //Resets timer to given time
    mTimer->start(ms);
}

void Controller::deviceShutDown(QListWidget *display)
{
    //TODO
    display->setStyleSheet("background-color:black;");
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

void Controller::togglePower()
{
    isPowerOn = !isPowerOn;
}
