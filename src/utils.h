#ifndef UTILS_H
#define UTILS_H
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

//https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
inline void delayMs(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
#endif // UTILS_H
