#include "earclips.h"

EarClips::EarClips(QObject *parent) : QObject(parent)
{
    rightEarClipConnection  = 0;
    leftEarClipConnection   = 0;
}

int EarClips::minConnectionLevel()
{
    return std::min(leftEarClipConnection, rightEarClipConnection);
}

bool EarClips::isLeftConnected()
{
    return leftEarClipConnection != 0;
}

bool EarClips::isRightConnected()
{
    return rightEarClipConnection != 0;
}

void EarClips::handleLeftEarClipSlider(int val)
{
    leftEarClipConnection = val;
}

void EarClips::handleRightEarClipSlider(int val)
{
    rightEarClipConnection = val;
}
