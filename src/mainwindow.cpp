#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // UI setup
    ui->setupUi(this);
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

    Battery *battery     = new Battery();
    controller           = new Controller(battery, groups); // TODO singleton
    EarClips *earClips   = new EarClips();

    connect(controller, SIGNAL(newRecord(Record *)), this, SLOT(handleNewRecord(Record *)));
    connect(ui->PowerButton, SIGNAL(clicked()), this, SLOT(handlePowerPressed()));

    // Initialize context
    this->context["sessionSelection"] = false;
    this->context["connectionTest"] = false;
    this->context["session"] = false;
    this->context["recordingSession"] = false;
    this->context["navigatingHistory"] = false;

    // Initialize timer
    // TODO move to constructor
    controller->initializeTimer(ui->listWidget);

    // Just for testing
    controller->recordSession();
    controller->recordSession();
    controller->recordSession();
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
