#include "controller.h"

Controller::Controller(Battery *b, QList<Group *> groups, QTimer *shTimer, QObject *parent) : QObject(parent)
{
    earClips       = nullptr;
    currentBattery = b;
    isPowerOn      = false;
    this->groups   = groups;
    shutDownTimer = shTimer;
    connect(shutDownTimer, &QTimer::timeout, [this]() { emit powerOnOff(); });
    shutDownTimer->start(5000);
}

Controller::~Controller()
{
    for (Record* record : history)
    {
        delete record;
    }

    for (Group* group: groups)
    {
        delete group;
    }
}

Record* Controller::recordSession()
{
    // TODO use actual Session
    Record* record = new Record(65, 5, SessionType::ALPHA);
    history.append(record);
    emit newRecord(record);
    return record;
}

//Initialize timer
void Controller::initializeTimer()
{
    shutDownTimer = new QTimer(this);
    connect(shutDownTimer, &QTimer::timeout, [this]() { emit powerOnOff(); });
    shutDownTimer->start(5000);
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
