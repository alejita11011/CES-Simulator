#include "group.h"

/**
 * @brief Representation of a group of sessions, where the name attribute specifies the planned time for the session.
 * @param sessions
 * @param name
 */
Group::Group(const QList<Session *> sessions, const QString name) : sessions(sessions), name(name)
{
}

QString Group::getName()
{
    return this->name;
}

QList<Session *> Group::getSessions()
{
    return this->sessions;
}

bool Group::containsSessionType(SessionType sessionType)
{
    for (Session *session : sessions)
    {
        if (session->getType() == sessionType)
        {
            return true;
        }
    }

    return false;
}

Group::~Group()
{
    for (Session* session: this->sessions)
    {
        delete session;
    }
}
