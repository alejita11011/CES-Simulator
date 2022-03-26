#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
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
