#include "earclips.h"

EarClips::EarClips(QObject *parent) : QObject(parent)
{
    isLeftClipConnected  = false;
    isRightClipConnected = false;
}

void EarClips::setLeftEarClip(bool status)
{
    isLeftClipConnected = status;
}

void EarClips::setRightEarClip(bool status)
{
    isRightClipConnected = status;
}

bool EarClips::isConnected(){   return (isLeftClipConnected && isRightClipConnected);   }
