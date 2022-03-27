#ifndef SESSION_H
#define SESSION_H

#include "sessiontype.h"

class Session
{
public:
    Session(bool pulseType, float frequency, int duration, SessionType type);
    bool getPulseType();
    float getFrequency();
    int getDuration();
    SessionType getType();

private:
    bool pulsetype;
    float frequency;
    int duration;
    SessionType type;
};

#endif // SESSION_H
