#ifndef EARCLIPS_H
#define EARCLIPS_H

#include <QObject>
#include <QString>

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
    // runs the ear clip connection test, 2 = excellent, 1 = okay, 0 = no connection
    // returns minimum of the vablues in variables rightEarClipConnection and leftEarClipConnection   
    int minConnectionLevel(); 
    bool isLeftConnected();
    bool isRightConnected();

private slots:
    void handleLeftEarClipSlider(int);
    void handleRightEarClipSlider(int);

private:
    int leftEarClipConnection;
    int rightEarClipConnection;

};

#endif // EARCLIPS_H
