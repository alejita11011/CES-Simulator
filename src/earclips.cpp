#include "earclips.h"

EarClips::EarClips(QObject *parent) : QObject(parent)
{
    rightEarClipConnection  = 0;
    leftEarClipConnection   = 0;
    earClipsConnectedDevice = false;
}

int EarClips::earClipConnectionTest()
{

    if (!earClipsConnectedDevice)
    {
        emit connectionLevel(0);
        return 0;
    }
    else if (leftEarClipConnection == 0 || rightEarClipConnection == 0)
    {
        emit connectionLevel(0);
        return 0;
    }
    int connectionValue = std::min(leftEarClipConnection, rightEarClipConnection);
    emit connectionLevel(connectionValue);
    return connectionValue;
}



void EarClips::handleEarClipConnection(int index)
{
    earClipsConnectedDevice = index;
    earClipConnectionTest();
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

