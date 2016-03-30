#ifndef EVENTSTORE_H
#define EVENTSTORE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSettings>
#include "store.h"
#include "event.h"

enum ListIndex {
    EventListIndex = 0,
    ObjectListIndex
};

class EventStore : public Store
{
    Q_OBJECT
public:
    EventStore(QObject *parent, QSettings *set_);
    void putEvent(Event* e_);    
    int checkChildObjExists(int objId_);
    void addTranslation(QString key_, QString trans_, int list_index_);
    QHash<QString,QString> l_event_names;
    QHash<QString,QString> l_object_names;

private:
    QList<QStandardItem*> prepareRow(QStandardItem* event_id, AnimatedStandardItem* severity_item, QStandardItem* param1, QStandardItem* param2, const QDateTime timestamp);
    void populateEventHash(QSqlDatabase* db_);
    void populateObjectHash(QSqlDatabase* db_);

    QSettings* settings;

public slots:
    void loadTranslationTable();

signals:
    void hashUpdated();
};

#endif // EVENTSTORE_H
