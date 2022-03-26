#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new Controller(); // TODO singleton

    connect(controller, SIGNAL(newRecord(Record*)), this, SLOT(handleNewRecord(Record*)));

    // Initialize context
    this->context["on"] = false;
    this->context["sessionSelection"] = false;
    this->context["connectionTest"] = false;
    this->context["session"] = false;
    this->context["recordingSession"] = false;
    this->context["navigatingHistory"] = false;

    qDebug().nospace() << "--> " << qPrintable(QString::number(this->context["session"]));

    // Just for testing
    controller->recordSession();
    controller->recordSession();
    controller->recordSession();
    ui->listWidget->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
}

QString MainWindow::formatSeconds(int seconds)
{
    int minutes = seconds / 60;
    if (minutes == 0) {
        return QString("%1s").arg(seconds);
    }
    int remainingSeconds = seconds % 60;
    if (remainingSeconds == 0) {
        return QString("%1m").arg(minutes);
    }
    return QString("%1m%2s")
            .arg(minutes)
            .arg(remainingSeconds, 2, 10, QChar('0'));
}

void MainWindow::handleNewRecord(Record *record)
{
    QString itemText = QString("%1 (%2)\n%3-session of intensity %4")
            .arg(record->getStartTime().toString("hh:mm:ss ap"))
            .arg(formatSeconds(record->getDuration()))
            .arg(ToString(record->getSessionType()))
            .arg(record->getIntensity());

    new QListWidgetItem(itemText, ui->listWidget);
//    ui->listWidget->addItem(&QListWidgetItem(itemText));
}
