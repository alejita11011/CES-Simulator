#include "record.h"

Record::Record(const int durationSeconds, const int intensity, const SessionType sessionType):
    startTime(QDateTime::currentDateTime().addSecs(-1 * durationSeconds)), durationSeconds(durationSeconds), intensity(intensity), sessionType(sessionType)
{
}

QDateTime Record::getStartTime()
{
    return startTime;
}

int Record::getDurationSeconds()
{
    return durationSeconds;
}

int Record::getIntensity()
{
    return intensity;
}

SessionType Record::getSessionType()
{
    return sessionType;
}
