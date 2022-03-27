#include "record.h"

Record::Record(const int duration, const int intensity, const SessionType sessionType):
    startTime(QDateTime::currentDateTime().addSecs(-1 * duration)), duration(duration), intensity(intensity), sessionType(sessionType)
{
}

QDateTime Record::getStartTime()
{
    return startTime;
}

int Record::getDuration()
{
    return duration;
}

int Record::getIntensity()
{
    return intensity;
}

SessionType Record::getSessionType()
{
    return sessionType;
}
