#ifndef EARCLIPS_H
#define EARCLIPS_H

#include <QObject>

/**
* ear clips are set to be disconnected during the initialization
* use setLeftEarClip(bool) and  setRightEarClip(bool)
* to connect the ear clips. (pass true to connect
* and false to disconnect)
* isConnected() returns true if both isLeftClipConnected
* and isRightClipConnected variables are true.
**/
class EarClips : public QObject
{
    Q_OBJECT
public:
    explicit EarClips(QObject *parent = nullptr);
    bool isConnected();
    void setRightEarClip(bool);
    void setLeftEarClip(bool);

signals:


private:
    bool isLeftClipConnected;
    bool isRightClipConnected;
};

#endif // EARCLIPS_H
