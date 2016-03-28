#ifndef STORE_H
#define STORE_H

#include <QObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include "treeviewfilterproxymodel.h"

class Store : public QObject
{
    Q_OBJECT
public:
    Store(QObject *parent);

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

protected:
    QStandardItemModel* model;
    TreeViewFilterProxyModel* proxy_model;

public slots:
    virtual void loadTranslationTable() {
        // Implement this if needed
    }

    friend class MainWindow;
    friend class ObjectView;
};

#endif // STORE_H
