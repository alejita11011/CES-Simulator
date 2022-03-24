#include "session.h"

Session::Session(bool pulseType, int frequency, int duration, SessionType type) {
    this->pulsetype = pulseType;
    this->frequency = frequency;
    this->duration  = duration;
    this->type      = type;
}

int Session::getFrequency() {
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
