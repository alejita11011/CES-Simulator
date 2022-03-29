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
    explicit Controller(Battery *b, QList<Group *> groups, QTimer *shTimer, QObject *parent = nullptr);
    ~Controller();
    Record* recordSession(); // TODO move to private
    void setEarClips(EarClips *);
    void changeBattery(Battery *);

    //Get power status
    bool getPowerStatus();

    void togglePower();

    //Reset Timeout timer
    void resetTimeout(int ms);


signals:
    void newRecord(Record* record);
    void powerOnOff();

private:
    QList<Record*> history;
    QTimer* shutDownTimer;
    EarClips *earClips;
    Battery *currentBattery;
    bool isPowerOn;
    QList<Group *> groups;
};

#endif // CONTROLLER_H
