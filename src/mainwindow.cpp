#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // UI setup
    ui->setupUi(this);
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
    //for testing
    controller->setEarClips(earClips);

    connect(controller, SIGNAL(sessionProgress(int, SessionType)), this, SLOT(handleSessionProgress(int, SessionType)));
    //Adjust Intensity
    connect(controller, SIGNAL(adjustSessionIntensity(int)), this, SLOT(handleIntensity(int)));
    connect(controller, SIGNAL(newRecord(Record *)), this, SLOT(handleNewRecord(Record *)));
    //A session ended
    connect(controller, SIGNAL(sessionEnds()), this, SLOT(handleEndedSession()));
    //reset Display
    connect(controller, SIGNAL(useSelectionContext()), this, SLOT(handleResetDisplay()));
    //When shutdown timer reaches 0 OR user turns off device
    connect(controller, SIGNAL(powerOff()), this, SLOT(handlePowerOff()));
    //User turns on device
    connect(controller, SIGNAL(powerOn(int, bool)), this, SLOT(handlePowerOn(int, bool)));
    connect(controller, SIGNAL(batteryLevel(bool)), this, SLOT(handleBattery(bool)));
    connect(controller, SIGNAL(batteryShutDown()), this, SLOT(handleBatteryShutDown()));

    connect(ui->PowerButton, SIGNAL(clicked()), controller, SLOT(handlePowerClicked()));
    connect(ui->SelectButton, SIGNAL(clicked()), controller, SLOT(handleSelectClicked()));
    //User connects/disconnects ear clips from device
    connect(ui->earClipDeviceCCBox, SIGNAL(currentIndexChanged(int)), controller, SLOT(handleEarClipConnection(int)));
    //Handle events for left ear clip slider
    connect(ui->leftEarClipSlider, SIGNAL(valueChanged(int)), earClips, SLOT(handleLeftEarClipSlider(int)));
    //Handle events for right ear clip slider
    connect(ui->rightEarClipSlider, SIGNAL(valueChanged(int)), earClips, SLOT(handleRightEarClipSlider(int)));
    //Handle signals from connection tests
    connect(controller, SIGNAL(sendEarClipConnection(int, bool, bool)), this, SLOT(handleConnectionTest(int, bool, bool)));
    //Handle connectionModeLight signals
    connect(controller,SIGNAL(connectionModeLight(bool)), this, SLOT(handleModeLight(bool)));
    //Handle battery change
    connect(ui->batteryChangeButton, SIGNAL(clicked()), this, SLOT(handleBatteryChange()));
    connect(ui->IntensityDown, SIGNAL(clicked()), controller, SLOT(handleDownClicked()));
    connect(ui->IntensityUp, SIGNAL(clicked()), controller, SLOT(handleUpClicked()));
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
    setLitUp({});
    ui->sessionProgressValues->raise();
    ui->sessionProgressValues->clear();
    ui->sessionProgressValues->setText(formatSeconds(remainingSeconds) + "\n" + ToString(sessionType));
}

void MainWindow::handleIntensity(int intensity)
{
    setLitUp({intensity});
}

void MainWindow::handleEndedSession(){
    //Graphs from 8-1
    delayMs(500);
    for(int end = 8; end >= 0; end--)
    {
        QSet<int> nums = {};
        for(int number = 1; number <= end; number++)
        {
            nums.insert(number);
        }
        setLitUp(nums);
        delayMs(500);
    }

    //Prompt user to record session
    ui->textValues->raise();
    ui->textValues->setText("Do you want to record the session?\nPress check mark/power");

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

void MainWindow::handleBattery(bool critical)
{
    if (critical)
    {
        setLitUp({1});
    }else{
        setLitUp({1,2});
    }
    delayMs(500);
    setLitUp({});
}

void MainWindow::handleBatteryShutDown()
{
    //Battery is critically low
    ui->textValues->raise();
    ui->textValues->setText("Battery critically low\nShutting down");
    ui->SelectButton->setDisabled(true);
    setLitUp({1});
    delayMs(2000);

}

// Power on (assuming not critically low)
void MainWindow::handlePowerOn(int batteryLevel, bool isLow)
{
    handleResetDisplay();

    ui->SelectButton->setDisabled(false);
    setLitUp(ui->powerLed, true);

    //Display battery level
    qDebug() << "BATTERY LEVEL" << batteryLevel;
    if(isLow){
        //Battery is low
        setLitUp({1,2});
    }else if (batteryLevel <= 75){
        setLitUp({1,2,3,4,5,6});
    }else{
        setLitUp({1,2,3,4,5,6,7,8});
    }

    delayMs(2000);
    setLitUp({});
}

void MainWindow::handleResetDisplay()
{
    if (ui->listWidget->count() == 0)
    {
        ui->textValues->raise();
        ui->textValues->setText("No records\nStart a session");
    } else
    {
        ui->listWidget->raise();
    }
    setLitUp({});
    handleGroupSelected();
}

void MainWindow::handlePowerOff()
{
    //Turn off numbers
    setLitUp({});

    // Turn off lights
    setLitUp(ui->powerLed, false);
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

void MainWindow::handleConnectionTest(int level, bool isLeftDisconnected, bool isRightDisconnected)
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
        //setLitUp({7,8});
        flash({7,8}, 3, 200);
        if (isLeftDisconnected && isRightDisconnected)
        {
            flash(ui->leftConnected, 3, 200);
            flash(ui->rightConnected, 3, 200);
        }
        else if (isLeftDisconnected)
        {
            flash(ui->leftConnected, 3, 200);

        }
        else if (isRightDisconnected)
        {
            flash(ui->rightConnected, 3, 200);
        }
    }
    delayMs(1000);
}

void MainWindow::handleModeLight(bool isShortPulse)
{
    if (isShortPulse)
    {
        flash(ui->shortPulse, 3, 100);
        return;
    }
    flash(ui->longPulse, 3, 200);
}

void MainWindow::handleBatteryChange()
{
    Battery *b = new Battery();
    controller->changeBattery(b);
    // let the user know battery has been changed?
    // display battery level
    // if this is pressed during active session
    // the session should be stopped.
}



void MainWindow::flash(QWidget *widget, int times, int onDurationMs)
{
    for (int i = 0; i < times; i++)
    {
        setLitUp(widget, true);
        delayMs(onDurationMs);
        setLitUp(widget, false);
    }
}

void MainWindow::flash(QSet<int> numbers, int times, int onDurationMs)
{
    for (int i = 0; i < times; i++)
    {
        setLitUp(numbers);
        delayMs(onDurationMs);
        setLitUp({});
    }
}
