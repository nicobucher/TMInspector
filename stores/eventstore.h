#ifndef EVENTSTORE_H
#define EVENTSTORE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QStringListModel>
#include "models/custommodels.h"
#include "store.h"
#include "event.h"
#include "models/eventmodel.h"
#include "translator.h"

enum ListIndex {
    EventListIndex = 0,
    ObjectListIndex,
    SPIDListIndex,
    PI_VALUESListIndex,
    PIC_VALUESListIndex
};

class EventModel;

class EventStore : public Store
{
    Q_OBJECT
public:
    EventStore(QObject *parent = 0);

    void setModelTimestampFmt(const QString &value);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel();

    int checkChildObjExists(int objId_);

    TreeViewFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {
        return this->proxy_model;
    }

    void setWatch_list(QStringListModel *value);

    QStandardItem* findItemInStore(QString obj_name);
    QStandardItem* findItemInStore(int obj_id);
private:
    EventModel* model;

    QList<QStandardItem*> prepareRow(Event* e_);

    QStringListModel* watch_list;

public slots:
    void exportToFile(QString filename_);
    void putEvent(Event* e_);

signals:
    void openView(int id, EventStore* store);
};

extern EventStore myEventStore;
extern EventStore mySqlEventStore;

#endif // EVENTSTORE_H
