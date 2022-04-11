#ifndef GROUP_H
#define GROUP_H

#include "session.h"
#include <QList>
#include <QString>

/**
*   The group represents a number of sessions with the same predetermiend time duration
*/
class Group
{
public:
    // Populate the session list and group's name
    Group(const QList<Session *> sessions, const QString name);

    // Return name attribute
    QString getName();

    // Get the session at the index passed through the parameter
    Session *getSession(int sessionIndex);

    // Returns the number of sessions in the group
    int size();

    // Returns true if a session in the group is of the type passed through the parameter
    bool containsSessionType(SessionType sessionType);

    // Deletes all sessions
    ~Group();

private:
    // List of session pointers
    const QList<Session *> sessions;

    // The name references the time duration of the group (e.g., "20min")
    const QString name;
};

#endif // GROUP_H
