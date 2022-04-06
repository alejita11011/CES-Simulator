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
    /**
     * @brief runs the ear clip connection test, 2 = excellent, 1 = okay, 0 = no connection
     * @return minimum of the vablues in variables rightEarClipConnection and leftEarClipConnection
     */
    int earClipConnectionTest();

signals:
    void connectionLevel(int, bool, bool);


private slots:
    void handleLeftEarClipSlider(int);
    void handleRightEarClipSlider(int);


private:
    int rightEarClipConnection;
    int leftEarClipConnection;

};

#endif // EARCLIPS_H
