#include "battery.h"

Battery::Battery(QObject *parent) : QObject(parent)
{
    percentage = 100;
}

void Battery::deplete(int amnt)
{
    percentage = percentage - amnt;
}

int Battery::getBatteryLevel() {   return percentage;   }
