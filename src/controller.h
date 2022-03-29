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
    void sessionProgress(int runningSeconds, SessionType type);

private slots:
    void handleSelectClicked();

private:
    QList<Record*> history;
    QTimer *clock;
    QTimer* mTimer;
    QTimer *remainingSessionTime;
    Session *currentSession;
    EarClips *earClips;
    Battery *currentBattery;
    bool isPowerOn;
    QList<Group *> groups;

    Record* recordSession(Session *session);
    void timerEvent(QTimerEvent *event);
};

#endif // CONTROLLER_H
