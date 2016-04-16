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
#include "eventmodel.h"

class EventModel;

enum ListIndex {
    EventListIndex = 0,
    ObjectListIndex
};

class EventStore : public Store
{
    Q_OBJECT
public:
    EventStore(QObject *parent, QSettings *set_, QHash<QString, QString> *l_objn_, QHash<QString, QString> *l_evn_);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel();

    void putEvent(Event* e_);
    int checkChildObjExists(int objId_);
    void clear_hash(int list_);

    QHash<QString,QString>* l_object_names;
    QHash<QString,QString>* l_event_names;

private:
    EventModel* model;

    QList<QStandardItem*> prepareRow(Event* e_);

    QSettings* settings;

public slots:
    void exportToFile(QString filename_);

signals:

};

#endif // EVENTSTORE_H
