#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QList>
#include <QListWidget>
#include <QTimer>
#include <QObject>
#include <QString>
#include <iostream>
#include "record.h"
#include "sessiontype.h"
#include "earclips.h"
#include "battery.h"
#include "session.h"
#include "group.h"
#include "utils.h"

/**
 *   @brief The Controller class handles the "business" logic of the device.
 */
class Controller : public QObject
{
    Q_OBJECT

/** Public members and methods **/
public:
    //populate the controller's attributes and set up the timer
    explicit Controller(Battery *b, QList<Group *> groups, EarClips *earClips, QObject *parent = nullptr);
    ~Controller();

    // Add a new battery object to the battery attribute
    void changeBattery(Battery *);

    // Time before the device turns off due to inactivity
    static int IDLE_TIMEOUT_MS;

/** All these signals are emitted by the controller to trigger the MainWindow to change **/
signals:
    // Send a record to be displayed in mainWindow
    void newRecord(Record *record);    
    
    // Refresh the touch screen showing the time left during the session
    void sessionProgress(int remainingSeconds, SessionType type, int batteryPercentage);
    
    // Notify the UI that the current session's intensity level has changed
    void adjustSessionIntensity(int intensity);

    // Notify the MainWindow that the current session has ended
    void sessionEnds();
    
    // Notify the ui to show the records history in the touch screen
    void displayRecords();

    // Notifies that the battery is low or critically low
    void batteryLevel(bool critical);
    
    // Notifies that the device is shutting down due to the critically low battery
    void batteryShutDown();
    
    // Notify the MainWindow that the CES needs to be powered off
    void powerOff();
    
    // Notifies the MainWindow that the power is ON as well as the current battery level
    // The MainWindow displays the battery level accordingly
    void powerOn(int batteryLevel, bool isLow);
    
    // Sends a signal indicating whether the short pulse or long pulse lights needs to be lit up
    // true == short pulse needs to be lit up, false == long pulse
    void connectionModeLight(bool);
    
    // Sends a signal indicating the level of connection 0-2 and 2 boolean values indiacating if 
    // either of the ear clips are completely disconnected.
    void sendEarClipConnection(int, bool, bool);

    // Send the group to be lit up in the screen
    void selectGroup(Group *group);

    // Send the session to be lit up in the number graph 1-8
    void selectSession(int selectedSessionIndex, Session *session);


/** All these slots are for the controller to react to some change on the MainWindow **/
private slots:
    // Handles the select (checkmark) button being clicked
    void handleSelectClicked();

    // Handles the power button being clicked, triggered when the button is pressed+released
    void handlePowerClicked();

    // Handles the power button being pressed, triggered when the button is initially pressed
    void handlePowerPressed();

    // Handles the ear clip sliders being changed, given the new slider value (0, 1, or 2)
    void handleEarClipsChanged(int);

    // Handles the selected options from the Connected/Disconnected drop down menu being changed   
    void handleEarClipsPluggedIn(int);

    // Handles the down button being clicked
    void handleDownClicked();

    // Handles the up button being clicked
    void handleUpClicked();

/** Private members and methods **/
private:
    // There are 4 possible contexts: sessionSelection, connectionTest, activeSession, promptRecordSession
    // depending on the context, the controller reacts to button clicks differently
    // in this map, if a context is mapped to true, that context is active
    QMap<QString, bool> context;

    // A list of Records representing past recorded sessions
    QList<Record *> history;

    // A list of the selectable Groups of sessions
    QList<Group *> groups;

    // A timer defaulting to IDLE_TIMEOUT_MS. When it reaches 0, the device shuts down
    QTimer *shutDownTimer;

    // A timer that is started when the power button is first pressed. If the power button is released, this timer will be stopped.
    // If the power button is not released for 2 seconds, the button will be considered to be held down
    QTimer *powerPressedTimer;

    // The session that is currently activated during the connectionTest and activeSession contexts
    // For other contexts, this will be set to nullptr
    Session *currentSession;

    // The ear clips of the device
    EarClips *earClips;

    // The battery of the device
    Battery *currentBattery;

    // Whether the device is powered on
    bool isPowerOn;

    // Group selected by the user - default 
    // In the range of 0-2 in the controller (fit an array)
    int selectedGroupIndex;

    // Session selected by the user - default 0
    // In the range of 0-3 in the controller (fit an array)
    int selectedSessionIndex;
    
    // Amount of time elapsed since the session started 
    int elapsedSessionTime;
    int timerId;

    // Boolean value representing wheather the earclips are connected to the CES device 
    bool earClipsAreConnected;

    // Current level of intensity for the current session
    int currentIntensity;
    
    // Highest intensity selected during a session - used for the records
    int highestIntensity;


    // A function invoked once every second
    // used during an activeSession context to update the timer on the MainWindow display
    // and monitor the battery
    void timerEvent(QTimerEvent *event);

    // getContext boolean - state of the context passed as parameter
    bool getContext(QString context);

    // Set the indicated context to true while making sure that other contexts are false. Returns whether the setter was successul or not
    bool setContext(QString context);

    // Make all contexts false (device is off)
    void resetContext();

    // Checks whether the connection level is good enough to continue a session
    void connectionTest();
    
    // Stops the current session and emits sessionEnds() signal
    void stopSession();

    // Stores the current session to the history attribute depending on input
    void stopRecordPrompt(bool shouldRecord);
    
    //Turns the device off if device was PREVIOUSLY on. Also, turns the device on if device was PREVIOUSLY off.
    void togglePower();
    
    // If during an active session the connection to the earclip is lost, calls the connectionTest() function then drops the intensity down to level 1
    void pauseSession();

    // Extends the time the device is on given some interaction with the device
    void resetShutDownTimer();
};

#endif // CONTROLLER_H
