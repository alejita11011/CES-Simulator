#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>

/**
* battery percentage is set to 100%
* at initialization. deplete() takes in
* an int and reduced battery percentage
* by the input amount. getBatteryLevel()
* returns the current percentage value.
**/
class Battery : public QObject
{
    Q_OBJECT
public:
    explicit Battery(QObject *parent = nullptr);
    void deplete(int);
    int getBatteryLevel();
    bool isLow();
    bool isCriticallyLow();

signals:

private:
    int percentage;

};

#endif // BATTERY_H
