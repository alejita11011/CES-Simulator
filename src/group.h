#ifndef GROUP_H
#define GROUP_H

#include "session.h"
#include <QList>
#include <QString>


class Group {
public:
    Group(QList<Session *> sessions, QString name);
    QString getName();
    QList<Session *> getSessions();
    ~Group();
private:
    QList<Session *> sessions;
    QString name;
};

#endif // GROUP_H
