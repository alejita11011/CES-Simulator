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
#include "utils.h"


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
    void newRecord(Record *record);
    void sessionProgress(int remainingSeconds, SessionType type);
    void adjustSessionIntensity(int intensity);
    void sessionEnds();
    void useSelectionContext();
    void batteryLevel(bool critical);
    void batteryShutDown();
    void powerOff();
    void powerOn(int batteryLevel, bool isLow);
    void connectionModeLight(bool);
    void sendEarClipConnection(int);
    void selectGroup(Group *group);
    void selectSession(int selectedSessionIndex);

private slots:
    void handleSelectClicked();
    void handlePowerClicked();
    void handleEarClipConnectionLevel(int);
    void handleEarClipConnection(int);
    void handleDownClicked();
    void handleUpClicked();

private:
    QMap<QString, bool> context;
    QList<Record *> history;
    QList<Group *> groups;
    QTimer *shutDownTimer;
    Session *currentSession;
    EarClips *earClips;
    Battery *currentBattery;
    bool isPowerOn;
    int selectedGroupIndex;
    int selectedSessionIndex;
    int elapsedSessionTime;
    int timerId;
    bool earClipsAreConnected;
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
    void resetShutDownTimer();

};

#endif // CONTROLLER_H
