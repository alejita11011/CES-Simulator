#include "earclips.h"

EarClips::EarClips(QObject *parent) : QObject(parent)
{
    rightEarClipConnection  = 0;
    leftEarClipConnection   = 0;
}

int EarClips::earClipConnectionTest()
{

    if (leftEarClipConnection == 0)
    {
        emit connectionLevel(0,"Left");
        return 0;
    }
    else if (rightEarClipConnection == 0)
    {
        emit connectionLevel(0, "Right");
        return 0;
    }
    int connectionValue = std::min(leftEarClipConnection, rightEarClipConnection);
    emit connectionLevel(connectionValue, "None");
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

