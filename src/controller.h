#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QList>
#include <QListWidget>
#include <QTimer>
#include <QObject>
#include "record.h"
#include "sessiontype.h"
#include "session.h"
#include "group.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    Record* recordSession(); // TODO move to private

    //Create Timer
    void initializeTimer(QListWidget* display);

    //Reset Timeout timer
    void resetTimeout(int ms);

    //Shutdown OASIS device
    void deviceShutDown(QListWidget* display ); // TODO

signals:
    void newRecord(Record* record);

private:
    QList<Record*> history;

    QTimer* mTimer;


};

#endif // CONTROLLER_H
