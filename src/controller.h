#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QList>
#include <QObject>
#include "record.h"
#include "sessiontype.h"
#include "session.h"
#include "group.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    Record* recordSession(); // TODO move to private

signals:
    void newRecord(Record* record);

private:
    QList<Record*> history;
};

#endif // CONTROLLER_H
