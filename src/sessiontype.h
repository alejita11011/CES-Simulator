#ifndef SESSIONTYPE_H
#define SESSIONTYPE_H

// The four supported session types by this device
enum SessionType { SUB_DELTA, DELTA, THETA, ALPHA };

// https://stackoverflow.com/questions/5093460
// Converts the given SessionType enum to a string representation
inline const char* ToString(SessionType sessionType)
{
    switch (sessionType)
    {
        case SUB_DELTA: return "sΔ";
        case DELTA: return "Δ";
        case THETA: return "Θ";
        case ALPHA: return "α";
        default: return "[Unknown SessionType]";
    }
}

#endif // SESSIONTYPE_H
