#include "group.h"

Group::Group(QList<Session *> sessions, QString name)
{
    this->name = name;
    this->sessions = sessions;
}

QString Group::getName()
{
    return this->name;
}

QList<Session *> Group::getSessions()
{
    return this->sessions;
}

Group::~Group()
{
    for (Session* session: this->sessions)
    {
        delete session;
    }
}
