#include "battery.h"

Battery::Battery(QObject *parent) : QObject(parent)
{
    percentage = 100;
}

void Battery::deplete(int amnt)
{
    if ((percentage - amnt) <= 0)
    {
        percentage = 0;
        return;
    }
    percentage = percentage - amnt;
}

int Battery::getBatteryLevel() {   return percentage;   }
