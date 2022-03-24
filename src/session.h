#ifndef SESSION_H
#define SESSION_H

#include "sessiontype.h"

class Session {
public:
    Session(bool pulseType, int frequency, int duration, SessionType type);
    bool getPulseType();
    int getFrequency();
    int getDuration();
    SessionType getType();

private:
    bool pulsetype;
    int frequency;  // Maybe this should be a float
    int duration;
    SessionType type;
};

#endif // SESSION_H
