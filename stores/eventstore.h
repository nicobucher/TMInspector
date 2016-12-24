#ifndef EVENTSTORE_H
#define EVENTSTORE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSettings>
#include "custommodels.h"
#include "store.h"
#include "event.h"
#include "eventmodel.h"
#include "translator.h"

class EventModel;

enum ListIndex {
    EventListIndex = 0,
    ObjectListIndex,
    SPIDListIndex,
    PI_VALUESListIndex,
    PIC_VALUESListIndex
};

class EventStore : public Store
{
    Q_OBJECT
public:
    EventStore(QObject *parent, QSettings *set_, EventTranslator* event_trans_, ObjectTranslator* obj_trans_);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel();

    void putEvent(Event* e_);
    int checkChildObjExists(int objId_);

    TreeViewFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {
        return this->proxy_model;
    }

    void setWatch_list(StringList *value);

    QStandardItem* findItemInStore(QString obj_id);
private:
    EventModel* model;

    QList<QStandardItem*> prepareRow(Event* e_);

    QSettings* settings;

    StringList* watch_list;

public slots:
    void exportToFile(QString filename_);

signals:
    void openView(QString name_);
};

#endif // EVENTSTORE_H
