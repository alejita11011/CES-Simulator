#include "controller.h"
#include <QDebug>
Controller::Controller(QObject *parent) : QObject(parent)
{
    Session *test0 = new Session(false, 3.5, 0, SessionType::ALPHA);
    Session *test1 = new Session(false, 3.5, 0, SessionType::ALPHA);
    Session *test2 = new Session(false, 3.5, 0, SessionType::ALPHA);

    QString name = "Hello";
    QList<Session *> *listTest = new QList<Session *>();

    listTest->append(test0);
    listTest->append(test1);
    listTest->append(test2);

    Group *testGroup = new Group(*listTest, name);

    qDebug().nospace() << "-->" << qPrintable(testGroup->getName());

    delete listTest;
    delete test0;
    delete test1;
    delete test2;
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
