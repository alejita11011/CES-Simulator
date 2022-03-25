#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QChar>
#include <QMap>
#include "controller.h"
#include "record.h"
#include "lightablewidget.h"
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
    QMap<QString, LightableWidget *> groupWidgets;
    // A map from SessionType to the LightableWidget associated with a session type
    QMap<SessionType, LightableWidget *> sessionWidgets;

    // Formats a number of seconds in ##m##s format
    QString formatSeconds(int seconds);

private slots:
    void handleNewRecord(Record* record);
};
#endif // MAINWINDOW_H
