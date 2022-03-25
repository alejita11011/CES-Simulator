#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->widget->setStyleSheet("background-color: rgb(0, 0, 0);");
    ui->setupUi(this);

//    groupWidgets["twenty"] = ui->twentyMinGroup;
//    groupWidgets["fourty"] = ui->fourtyFiveMinGroup;
//    groupWidgets["user"] = ui->userDefinedGroup;
//    sessionWidgets[SessionType::SUB_DELTA] = ui->subDeltaSession;
//    sessionWidgets[SessionType::DELTA] = ui->deltaSession;
//    sessionWidgets[SessionType::ALPHA] = ui->alphaSession;
//    sessionWidgets[SessionType::THETA] = ui->thetaSession;

    ui->twentyMinGroup->setStyleSheet("border-image: url(:/images/off/20.png);");
    qDebug() << ui->twentyMinGroup->styleSheet();
    controller = new Controller(); // TODO singleton

    connect(controller, SIGNAL(newRecord(Record*)), this, SLOT(handleNewRecord(Record*)));

    // Just for testing
    controller->recordSession();
    controller->recordSession();
    controller->recordSession();
    ui->listWidget->setCurrentRow(0);

//    groupWidgets["fourty"]->setLighted(true);
//    sessionWidgets[SessionType::DELTA]->setLighted(true);
//    ui->shortPulse->setLighted(true);
//    ui->rightConnected->setLighted(true);
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
