#include "controller.h"

Controller::Controller(Battery *b, QList<Group *> groups, QObject *parent) : QObject(parent)
{
    earClips       = nullptr;
    currentBattery = b;
    isPowerOn      = false;
    this->groups   = groups;

    // Initialize context
    this->context["sessionSelection"] = false;
    this->context["connectionTest"] = false;
    this->context["session"] = false;
    this->context["recordingSession"] = false;
    this->context["navigatingHistory"] = false;
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
