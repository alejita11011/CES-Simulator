#include "session.h"

/**
 * @brief Session::Session
 * @param isShortPulse
 * @param frequency
 * @param duration
 * @param type
 */
Session::Session(const bool shortPulse, const float frequency, const int presetDurationSeconds, const SessionType type) :
    shortPulse(shortPulse), frequency(frequency), presetDurationSeconds(presetDurationSeconds), type(type)
{
}

float Session::getFrequency()
{
    return this->frequency;
}

int Session::getPresetDurationSeconds()
{
    return this->presetDurationSeconds;
}

bool Session::isShortPulse()
{
    return this->shortPulse;
}

SessionType Session::getType()
{
    return this->type;
}
