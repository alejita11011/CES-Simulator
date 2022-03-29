#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QList>
#include <QListWidget>
#include <QTimer>
#include <QObject>
#include "record.h"
#include "sessiontype.h"
#include "earclips.h"
#include "battery.h"
#include "session.h"
#include "group.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Battery *b, QList<Group *> groups, QObject *parent = nullptr);
    ~Controller();
    Record* recordSession(); // TODO move to private
    void setEarClips(EarClips *);
    void changeBattery(Battery *);
    void togglePower();

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
    EarClips *earClips;
    Battery *currentBattery;
    bool isPowerOn;
    QList<Group *> groups;
    QMap<QString, bool> context;
};

#endif // CONTROLLER_H
