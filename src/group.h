#ifndef GROUP_H
#define GROUP_H

#include "session.h"
#include <QList>
#include <QString>


class Group
{
public:
    Group(const QList<Session *> sessions, const QString name);
    QString getName();
    Session* getSessions(int sessionIndex);
    bool containsSessionType(SessionType sessionType);
    ~Group();

private:
    const QList<Session *> sessions;
    const QString name;
};

#endif // GROUP_H
