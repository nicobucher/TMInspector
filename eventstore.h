#ifndef EVENTSTORE_H
#define EVENTSTORE_H

#include <QObject>
#include "store.h"
#include "event.h"

class EventStore : public Store
{
public:
    EventStore(QObject *parent);
    void putEvent(Event* e_);    
    int checkChildObjExists(int objId_);
    int checkChildObjExists(QStandardItem *objId_);
    QHash<QString,QString> l_event_names;
    QHash<QString,QString> l_object_names;

private:
    QList<QStandardItem*> prepareRow(QStandardItem* event_id, const Severity severity, QStandardItem* param1, QStandardItem* param2, const QDateTime timestamp);

public slots:
    void loadTranslationTable();
};

#endif // EVENTSTORE_H
