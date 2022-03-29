#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // UI setup
    ui->setupUi(this);
    this->setStyleSheet("MainWindow{background-image: url(:/images/CES.png); background-position: center;}");
    groupWidgets["20min"] = ui->twentyMinGroup;
    groupWidgets["45min"] = ui->fourtyFiveMinGroup;
    groupWidgets["user"] = ui->userDefinedGroup;
    sessionWidgets[SessionType::SUB_DELTA] = ui->subDeltaSession;
    sessionWidgets[SessionType::DELTA] = ui->deltaSession;
    sessionWidgets[SessionType::ALPHA] = ui->alphaSession;
    sessionWidgets[SessionType::THETA] = ui->thetaSession;


    // Create Sessions
    Group *twenty = new Group({new Session(true, 0.5, 20, SessionType::SUB_DELTA),
                            new Session(false, 2.5, 20, SessionType::DELTA),
                            new Session(false, 6, 20, SessionType::THETA),
                            new Session(false, 9, 20, SessionType::ALPHA)},
                           "20min");

    Group *fourtyFive = new Group({new Session(true, 0.5, 45, SessionType::SUB_DELTA),
                                   new Session(false, 2.5, 45, SessionType::DELTA),
                                   new Session(false, 6, 45, SessionType::THETA),
                                   new Session(false, 9, 45, SessionType::ALPHA),},
                                  "45min");

    Group *user = new Group({},"user");

    // Create the groups
    QList<Group *> groups = {twenty, fourtyFive, user};

    //Create battery
    Battery *battery     = new Battery();

    //Create shut down timer
    QTimer *shutDownTimer = new QTimer(this);
    //connect(shutDownTimer, &QTimer::timeout, [this, display]() { deviceShutDown(display); });

    //Create controller
    controller           = new Controller(battery, groups, shutDownTimer); // TODO singleton

    //Create earClips
    EarClips *earClips   = new EarClips();

    connect(controller, SIGNAL(sessionProgress(int, SessionType)), this, SLOT(handleSessionProgress(int, SessionType)));
    connect(controller, SIGNAL(newRecord(Record *)), this, SLOT(handleNewRecord(Record *)));
    connect(controller, SIGNAL(powerOnOff()), this, SLOT(handlePowerClicked()));
    connect(ui->PowerButton, SIGNAL(clicked()), this, SLOT(handlePowerClicked()));
    connect(ui->SelectButton, SIGNAL(clicked()), controller, SLOT(handleSelectClicked()));

    // Just for testing
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
    if (minutes == 0)
    {
        return QString("%1s").arg(seconds);
    }
    int remainingSeconds = seconds % 60;
    if (remainingSeconds == 0)
    {
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
            .arg(formatSeconds(record->getDurationSeconds()))
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

//Displays session progress on device screen
void MainWindow::handleSessionProgress(int elapsedSeconds, SessionType sessionType)
{
    ui->sessionProgressValues->raise();
    ui->sessionProgressValues->clear();
    ui->sessionProgressValues->setText(formatSeconds(elapsedSeconds) + "\n" + ToString(sessionType));
}

void MainWindow::setLitUp(QWidget *widget, bool litUp)
{
    QString styleSheet = widget->styleSheet();
    widget->setStyleSheet(styleSheet.replace(litUp ? "/off/" : "/on/", litUp ? "/on/" : "/off/"));
}

//REVIEW
void MainWindow::handlePowerClicked()
{
    controller->togglePower();

    if(controller->getPowerStatus()){
        //Shut down device
        ui->powerOnOffView->raise();

    }else if( controller->getPowerStatus() == false){
        //Turn on device
        ui->listWidget->raise();
    }
}

