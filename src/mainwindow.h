#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QChar>
#include <QMap>
#include <QLabel>
#include <QSet>
#include <iostream>
#include "utils.h"
#include "controller.h"
#include "record.h"
#include "earclips.h"
#include "battery.h"
#include "sessiontype.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Controller *controller;
    // A map from group name to the LightableWidget associated with a Group
    QMap<QString, QWidget *> groupWidgets;
    // A map from SessionType to the LightableWidget associated with a session type
    QMap<SessionType, QWidget *> sessionWidgets;
    // A list of the number labels
    QList<QLabel *> numberLabels;

    // Formats a number of seconds in ##m##s format
    QString formatSeconds(int seconds);

    void setLitUp(QWidget *widget, bool litUp);
    void setLitUp(QSet<int> numbers);
    void flash(QWidget *, int, int);
    void flash(QSet<int> , int, int);

private slots:
    void handleNewRecord(Record* record);  

    void handleGroupSelected(Group *group);

    // Lights up the selected session's number in the graph and it's respective pulse light
    void handleSessionSelected(int selectedSessionIndex, Session *selectedSession);

    void handleSessionProgress(int elapsedSeconds, SessionType sessionType, int batteryPercentage);

    // Handles the adjustSessionIntensity signal and displays
    // the new intensity level in the UI
    void handleIntensity(int intensity);

    void handleEndedSession();

    void handleRecordsDisplay();

    void handleBattery(bool);

    void handleBatteryShutDown();

    void handlePowerOff();

    void handlePowerOn(int, bool);

    // Handles the sendEarClipConnection signal from Controller
    // displays the connection level in UI and if one or more of the
    // ear clips are disconnected, lights up the respective icons 
    void handleConnectionTest(int, bool, bool);

    // Handles the connectionModeLight signal from Controller
    // lights up the short pulse icon if input parameter is true,
    // lights up the long pulse icon otherwise 
    void handleModeLight(bool);

    // Handles the change battery button clicked from the MainWindow
    // by passing the controller a new battery to use
    void handleBatteryChange();

};
#endif // MAINWINDOW_H
