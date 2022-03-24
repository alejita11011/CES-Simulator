#include "controller.h"

Controller::Controller(QObject *parent, Battery *b) : QObject(parent)
{
    earClips       = nullptr;
    currentBattery = b;
}

Controller::~Controller()
{
    for (Record* record : history) {
        delete record;
    }
}

Record* Controller::recordSession()
{
    // TODO use actual Session

//    Record* record = new Record(20 * 60, 5, SessionType::ALPHA);
    Record* record = new Record(65, 5, SessionType::ALPHA);
    history.append(record);
    emit newRecord(record);
    return record;
}

void Controller::setEarClips(EarClips *e)
{
    earClips = e;
}

void Controller::changeBattery(Battery *b)
{
    delete currentBattery;
    currentBattery = b;
}
