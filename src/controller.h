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

    static int IDLE_TIMEOUT_MS;

signals:
    void newRecord(Record* record);
    void sessionProgress(int remainingSeconds, SessionType type);
    void adjustSessionIntensity(int intensity);
    void sessionEnds();
    void useSelectionContext();
    void powerOff();
    void powerOn();

private slots:
    void handleSelectClicked();
    void handlePowerClicked();
    void handleDownClicked();
    void handleUpClicked();

private:
    QMap<QString, bool> context;
    QList<Record*> history;
    QList<Group *> groups;
    QTimer *shutDownTimer;
    Session *currentSession;
    EarClips *earClips;
    Battery *currentBattery;
    bool isPowerOn;
    int elapsedSessionTime;
    int timerId;
    int currentIntensity;
    int highestIntensity;

    void timerEvent(QTimerEvent *event);

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

    void stopSession();
    void stopRecordPrompt(bool shouldRecord);
    void togglePower();
};

#endif // CONTROLLER_H
