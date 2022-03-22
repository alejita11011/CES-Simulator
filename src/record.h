#ifndef RECORD_H
#define RECORD_H

#include "sessiontype.h"

class Record
{
public:
    Record(const int duration, const int intensity, const SessionType sessionType);
    const int getDuration();
    const int getIntensity();
    const int getSessionType();

private:
    const int duration;
    const int intensity;
    const SessionType sessionType;
};

#endif // RECORD_H
