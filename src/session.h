#ifndef SESSION_H
#define SESSION_H

class Session {
public:
    Session(bool pulseType, int frequency, int duration);
    bool getPulseType();
    int getFrequency();
    int getDuration();

private:
    bool pulsetype;
    int frequency;
    int duration;
};

#endif // SESSION_H
