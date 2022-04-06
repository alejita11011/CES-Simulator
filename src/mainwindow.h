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

private slots:
    void handleNewRecord(Record* record);  

    void handleGroupSelected(/* Group *group */);

    void handleSessionProgress(int elapsedSeconds, SessionType sessionType);

    void handleIntensity(int intensity);

    void handleEndedSession();

    void handleResetDisplay();

    void handleBattery(bool);

    void handleBatteryShutDown();

    void handlePowerOff();

    void handlePowerOn(int, bool);

    void handleConnectionTest(int, bool, bool);

    void handleModeLight(bool);

    void handleBatteryChange();


};
#endif // MAINWINDOW_H
