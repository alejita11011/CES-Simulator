#include "session.h"

/**
 * @brief Session::Session
 * @param pulseType
 * @param frequency
 * @param duration
 * @param type
 */
Session::Session(bool pulseType, float frequency, int duration, SessionType type) {
    this->pulsetype = pulseType;
    this->frequency = frequency;
    this->duration  = duration;
    this->type      = type;
}

float Session::getFrequency() {
    return this->frequency;
}

int Session::getDuration() {
    return this->duration;
}

bool Session::getPulseType() {
    return this->pulsetype;
}

SessionType Session::getType() {
    return this->type;
}
