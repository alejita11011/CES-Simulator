#include "group.h"

/**
 * @brief Representation of a group of sessions, where the name attribute specifies the planned time for the session.
 * @param sessions
 * @param name
 */
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
