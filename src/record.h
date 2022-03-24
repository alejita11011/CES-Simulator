#ifndef RECORD_H
#define RECORD_H

#include <QDateTime>
#include "sessiontype.h"

class Record
{
public:
    Record(const int duration, const int intensity, const SessionType sessionType);
    QDateTime getStartTime();
    int getDuration();
    int getIntensity();
    SessionType getSessionType();

private:
    const QDateTime startTime;
    const int duration;
    const int intensity;
    const SessionType sessionType;
};

#endif // RECORD_H
