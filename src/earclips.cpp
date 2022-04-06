#include "earclips.h"

EarClips::EarClips(QObject *parent) : QObject(parent)
{
    rightEarClipConnection  = 0;
    leftEarClipConnection   = 0;
}

int EarClips::earClipConnectionTest()
{
    if (leftEarClipConnection == 0 && rightEarClipConnection == 0)
    {
        emit connectionLevel(0, true, true);
    }
    else if (leftEarClipConnection == 0)
    {
        emit connectionLevel(0, true, false);
        return 0;
    }
    else if (rightEarClipConnection == 0)
    {
        emit connectionLevel(0, false, true);
        return 0;
    }
    int connectionValue = std::min(leftEarClipConnection, rightEarClipConnection);
    emit connectionLevel(connectionValue, false, false);
    return connectionValue;
}

void EarClips::handleLeftEarClipSlider(int val)
{
    leftEarClipConnection = val;
    earClipConnectionTest();
}

void EarClips::handleRightEarClipSlider(int val)
{
    rightEarClipConnection = val;
    earClipConnectionTest();
}

