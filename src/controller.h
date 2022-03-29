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

    void setEarClips(EarClips *);
    void changeBattery(Battery *);

    //Get power status
    bool getPowerStatus();

    void togglePower();

    //Reset Timeout timer
    void resetTimeout(int ms);


    /**
     * @brief getContext
     * @param context
     * @return boolean - state of the context passed as parameter
     */
    bool getContext(QString context);

    /**
     * @brief Set the indicated context to true while making sure that other contexts are false.
     * @param context
     * @return Whether the setter was successul or not
     */
    bool setContext(QString context);

    /**
     * @brief Make all contexts false (device is off)
     */
    void resetContext();

signals:
    void newRecord(Record* record);
    void sessionProgress(int runningSeconds, SessionType type);

private slots:
    void handleSelectClicked();

private:
    QMap<QString, bool> context;
    QList<Record*> history;
    QList<Group *> groups;
    QTimer *clock;
    QTimer* shutDownTimer;
    QTimer *remainingSessionTime;
    Session *currentSession;
    EarClips *earClips;
    Battery *currentBattery;
    bool isPowerOn;

    void timerEvent(QTimerEvent *event);
    Record* recordSession(Session *session);
    void powerOnOff();
};

#endif // CONTROLLER_H
