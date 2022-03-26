#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QList>
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
    explicit Controller(Battery *b, QObject *parent = nullptr);
    ~Controller();
    Record* recordSession(); // TODO move to private
    void setEarClips(EarClips *);
    void changeBattery(Battery *);
    void togglePower();

signals:
    void newRecord(Record* record);

private:
    QList<Record*> history;
    EarClips *earClips;
    Battery *currentBattery;
    bool power; //true == power on, false == power off
};

#endif // CONTROLLER_H
