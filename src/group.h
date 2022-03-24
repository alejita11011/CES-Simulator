#ifndef GROUP_H
#define GROUP_H

#include "session.h"
#include <QList>

class Group {
public:
    Group();
    ~Group();
private:
    QList<Session> sessions;
};

#endif // GROUP_H
