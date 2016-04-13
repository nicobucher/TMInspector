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
    EventStore(QObject *parent, QSettings *set_, QHash<QString, QString> *l_objn_, QHash<QString, QString> *l_evn_);

    bool itemInStore(QString obj_id) {
        QList<QStandardItem*> list = this->model->findItems(obj_id);
        if(list.length() > 0) {
            return true;
        } else {
            return false;
        }
    }

    int getNumberOfItems() {
        return this->model->rowCount();
    }

    void emptyStore() {
        this->model->removeRows(0, this->model->rowCount());
    }

    QStandardItemModel* getModel() {
        return this->model;
    }

    void putEvent(Event* e_);    
    int checkChildObjExists(int objId_);
    void clear_hash(int list_);

private:
    QStandardItemModel* model;

    QList<QStandardItem*> prepareRow(Event* e_);
    QHash<QString,QString>* l_object_names;
    QHash<QString,QString>* l_event_names;

    QSettings* settings;

public slots:
    void exportToFile(QString filename_);

signals:

};

#endif // EVENTSTORE_H
