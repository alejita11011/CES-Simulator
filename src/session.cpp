#include "session.h"

/**
 * @brief Session::Session
 * @param pulseType
 * @param frequency
 * @param duration
 * @param type
 */
Session::Session(const bool pulseType, const float frequency, const int presetDurationSeconds, const SessionType type) :
    pulseType(pulseType), frequency(frequency), presetDurationSeconds(presetDurationSeconds), type(type)
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

bool Session::getPulseType()
{
    return this->pulseType;
}

SessionType Session::getType()
{
    return this->type;
}
