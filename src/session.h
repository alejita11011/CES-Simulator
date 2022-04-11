#ifndef SESSION_H
#define SESSION_H

#include "sessiontype.h"

/**
* A session is a pre-configured set of attributes describing a electrical stimulation therapy.
* It is immutable.
*/
class Session
{
// Constructor and getters
public:
    Session(const bool pulseType, const float frequency, const int presetDurationSeconds, const SessionType type);
    bool isShortPulse();
    float getFrequency();
    int getPresetDurationSeconds();
    SessionType getType();

private:
    // Whether this session uses short pulses (true) or long pulses (false)
    const bool shortPulse;

    // The frequency that this session uses
    const float frequency;

    // The amount of time that this session is configured to run for. Users may end the session early.
    const int presetDurationSeconds;

    // The type of the session
    const SessionType type;
};

#endif // SESSION_H
