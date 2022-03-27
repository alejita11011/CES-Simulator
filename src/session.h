#ifndef SESSION_H
#define SESSION_H

#include "sessiontype.h"

class Session
{
public:
    Session(const bool pulseType, const float frequency, const int presetDurationSeconds, const SessionType type);
    bool getPulseType();
    float getFrequency();
    int getPresetDurationSeconds();
    SessionType getType();

private:
    const bool pulseType;
    const float frequency;
    const int presetDurationSeconds;
    const SessionType type;
};

#endif // SESSION_H
