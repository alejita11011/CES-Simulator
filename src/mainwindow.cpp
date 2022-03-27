#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Battery *battery     = new Battery();
    controller           = new Controller(battery); // TODO singleton
    EarClips *earClips   = new EarClips();


    groupWidgets["twenty"] = ui->twentyMinGroup;
    groupWidgets["fourty"] = ui->fourtyFiveMinGroup;
    groupWidgets["user"] = ui->userDefinedGroup;
    sessionWidgets[SessionType::SUB_DELTA] = ui->subDeltaSession;
    sessionWidgets[SessionType::DELTA] = ui->deltaSession;
    sessionWidgets[SessionType::ALPHA] = ui->alphaSession;
    sessionWidgets[SessionType::THETA] = ui->thetaSession;


    connect(controller, SIGNAL(newRecord(Record*)), this, SLOT(handleNewRecord(Record*)));
    connect(ui->PowerButton, SIGNAL(clicked()), this, SLOT(handlePowerPressed()));

    // Initialize context
    this->context["sessionSelection"] = false;
    this->context["connectionTest"] = false;
    this->context["session"] = false;
    this->context["recordingSession"] = false;
    this->context["navigatingHistory"] = false;

    // Just for testing
    controller->recordSession();
    controller->recordSession();
    controller->recordSession();

    //Initilize timer
    controller->initializeTimer(ui->listWidget);
    ui->listWidget->setCurrentRow(0);

    handleGroupSelected();
    setLitUp(ui->leftConnected, true);
    setLitUp(ui->rightConnected, true);
    setLitUp(ui->shortPulse, true);
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
}

void MainWindow::handleGroupSelected(/* Group *group */)
{
    setLitUp(ui->fourtyFiveMinGroup, true);
    setLitUp(ui->subDeltaSession, true);
    setLitUp(ui->deltaSession, true);
    setLitUp(ui->alphaSession, true);
    setLitUp(ui->thetaSession, true);
}

void MainWindow::setLitUp(QWidget *widget, bool litUp)
{
    QString styleSheet = widget->styleSheet();
    widget->setStyleSheet(styleSheet.replace(litUp ? "/off/" : "/on/", litUp ? "/on/" : "/off/"));
}

//REVIEW
void MainWindow::on_PowerButton_clicked()
{
    controller->deviceShutDown(ui->listWidget);
}

void MainWindow::handlePowerPressed()
{
    controller->togglePower();
    // TO_DO
    // turn off GUI
}
