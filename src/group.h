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
    QList<Session *> getSessions();
    ~Group();

private:
    const QList<Session *> sessions;
    const QString name;
};

#endif // GROUP_H
