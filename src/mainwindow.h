#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QChar>
class Controller;
#include "controller.h"
#include "record.h"
#include "earclips.h"
#include "battery.h"

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
    QString formatSeconds(int seconds);
    EarClips *earClips;
    Battery *battery;

private slots:
    void handleNewRecord(Record* record);
    void handlePowerPressed();
};
#endif // MAINWINDOW_H
