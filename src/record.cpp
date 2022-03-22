#include "record.h"

Record::Record(const int duration, const int intensity, const SessionType sessionType):
    duration(duration), intensity(intensity), sessionType(sessionType)
{
}

const int Record::getDuration() {
    return duration;
}

const int Record::getIntensity() {
    return intensity;
}

const int Record::getSessionType() {
    return sessionType;
}
