#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // UI setup
    ui->setupUi(this);
    //this->setStyleSheet("MainWindow{background-image: url(:/images/CES.png); background-position: center;}");
    this->setStyleSheet("QWidget#CESwidget{border-image: url(:/images/CES.png); background-position: center;}");
    groupWidgets["20min"] = ui->twentyMinGroup;
    groupWidgets["45min"] = ui->fourtyFiveMinGroup;
    groupWidgets["user"] = ui->userDefinedGroup;
    sessionWidgets[SessionType::SUB_DELTA] = ui->subDeltaSession;
    sessionWidgets[SessionType::DELTA] = ui->deltaSession;
    sessionWidgets[SessionType::ALPHA] = ui->alphaSession;
    sessionWidgets[SessionType::THETA] = ui->thetaSession;

    // Add number labels
    for (int i = 8; i >= 1; i--)
    {
        QLabel *label = new QLabel(QString("|%1|").arg(i));
        label->setAlignment(Qt::AlignHCenter);
        ui->numbersLayout->addWidget(label);
        //Add to list
        numberLabels.prepend(label);
    }

    handlePowerOff();

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


    //Create controller
    controller           = new Controller(battery, groups); // TODO singleton

    //Create earClips
    EarClips *earClips   = new EarClips();

    connect(controller, SIGNAL(sessionProgress(int, SessionType)), this, SLOT(handleSessionProgress(int, SessionType)));
    connect(controller, SIGNAL(newRecord(Record *)), this, SLOT(handleNewRecord(Record *)));
    //A session ended
    connect(controller, SIGNAL(sessionEnds()), this, SLOT(handleEndedSession()));
    //reset Display
    connect(controller, SIGNAL(useSelectionContext()), this, SLOT(handleResetDisplay()));
    //When shutdown timer reaches 0 OR user turns off device
    connect(controller, SIGNAL(powerOff()), this, SLOT(handlePowerOff()));
    //User turns on device
    connect(controller, SIGNAL(powerOn()), this, SLOT(handlePowerOn()));
    connect(ui->PowerButton, SIGNAL(clicked()), controller, SLOT(handlePowerClicked()));
    connect(ui->SelectButton, SIGNAL(clicked()), controller, SLOT(handleSelectClicked()));
    //User connects/disconnects ear clips from device
    connect(ui->earClipDeviceCCBox, SIGNAL(currentIndexChanged(int)), controller, SLOT(handleEarClipConnection(int)));
    //Handle events for left ear clip slider
    connect(ui->leftEarClipSlider, SIGNAL(valueChanged(int)), controller, SLOT(handleLeftEarClipSlider(int)));
    //Handle events for right ear clip slider
    connect(ui->rightEarClipSlider, SIGNAL(valueChanged(int)), controller, SLOT(handleRightEarClipSlider(int)));
    //Handle signals from connection tests
    connect(controller, SIGNAL(connectionLevel(int)), this, SLOT(handleConnectionTest(int)));
    // handle
    connect(controller,SIGNAL(connectionModeLight(bool)), this, SLOT(handleModeLight(bool)));

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

void MainWindow::handleGroupSelected(/* Group *group */) // TODO
{
    setLitUp(ui->fourtyFiveMinGroup, true);
    setLitUp(ui->subDeltaSession, true);
    setLitUp(ui->deltaSession, true);
    setLitUp(ui->alphaSession, true);
    setLitUp(ui->thetaSession, true);
}

//Displays session progress on device screen
void MainWindow::handleSessionProgress(int remainingSeconds, SessionType sessionType)
{
    ui->sessionProgressValues->raise();
    ui->sessionProgressValues->clear();
    ui->sessionProgressValues->setText(formatSeconds(remainingSeconds) + "\n" + ToString(sessionType));
}

void MainWindow::handleEndedSession(){
    //Graphs from 8-1
    for(int end = 8; end >= 0; end--)
    {
        QSet<int> nums = {};
        for(int number = 1; number <= end; number++)
        {
            nums.insert(number);
        }
        setLitUp(nums);
        delay(500);
    }

    //Prompt user to record session
    //Check mark : Yes
    //Power button : No
    ui->sessionProgressValues->setText("Do you want to record the session?\nPress ✅/🔋");
}

void MainWindow::handleResetDisplay()
{
    ui->listWidget->raise();
    setLitUp({});
    handleGroupSelected();
}

//https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
void MainWindow::delay(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::setLitUp(QWidget *widget, bool litUp)
{
    QString styleSheet = widget->styleSheet();
    widget->setStyleSheet(styleSheet.replace(litUp ? "/off/" : "/on/", litUp ? "/on/" : "/off/"));
}

// Lights up the set of numbers passed, and turns off the rest
void MainWindow::setLitUp(QSet<int> numbers)
{
    for (int number = 1; number <= 8; number++)
    {
        QLabel *label = numberLabels[number - 1];

        if (numbers.contains(number))
        {
            // Set label to colour
            if (number == 8 || number == 7)
            {
                label->setStyleSheet("background-color:rgb(174, 0, 0);");
            }
            else if (number == 6 || number == 5 || number == 4)
            {
                label->setStyleSheet("background-color:rgb(249, 166, 0);");
            }
            else
            {
                label->setStyleSheet("background-color:rgb(233, 233, 0);");
            }
        }
        else
        {
            label->setStyleSheet("background-color:rgb(130, 130, 130);");
        }
    }
}

void MainWindow::handlePowerOn()
{
    ui->listWidget->raise();
}

void MainWindow::handlePowerOff()
{
    //Turn off numbers
    setLitUp({});

    // Turn off lights
    for (QWidget *groupWidget : groupWidgets.values())
    {
        setLitUp(groupWidget, false);
    }
    for (QWidget *sessionWidget : sessionWidgets.values())
    {
        setLitUp(sessionWidget, false);
    }
    setLitUp(ui->shortPulse, false);
    setLitUp(ui->longPulse, false);
    setLitUp(ui->leftConnected, false);
    setLitUp(ui->rightConnected, false);

    // Turn off display
    ui->powerOffView->raise();
}

void MainWindow::handleConnectionTest(int level)
{
    if (level == 2)
    {
        setLitUp({1,2,3});
    }
    else if (level == 1)
    {
        setLitUp({4,5,6});
    }
    else
    {
        setLitUp({7,8});
    }
}

void MainWindow::handleModeLight(bool isShortPulse)
{
    if (isShortPulse)
    {
        setLitUp(ui->shortPulse, true);
        return;
    }
    setLitUp(ui->longPulse, true);
}





