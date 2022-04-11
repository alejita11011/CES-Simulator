#ifndef RECORD_H
#define RECORD_H

#include <QDateTime>
#include "sessiontype.h"

/**
* A Record is a record of a previous session that the user ran and requested to be recorded.
* A record keeps track of the duration, highest intensity selected during a session, 
* and the session type of the session recorded.
**/

class Record
{
public:
    Record(const int durationSeconds, const int intensity, const SessionType sessionType);
    QDateTime getStartTime();
    int getDurationSeconds();
    int getIntensity();
    SessionType getSessionType();

private:
    // Records the Day time in which a session started
    const QDateTime startTime;
    const int durationSeconds;
    const int intensity;
    const SessionType sessionType;
};

#endif // RECORD_H
