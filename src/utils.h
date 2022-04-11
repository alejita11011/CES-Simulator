#ifndef UTILS_H
#define UTILS_H
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

// https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
// Delays for the given amount of milliseconds (non-blocking)
inline void delayMs(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
#endif // UTILS_H
