#ifndef STORE_H
#define STORE_H

#include <QObject>
#include <QStandardItem>
#include <QStandardItemModel>

class Store : public QObject
{
    Q_OBJECT
public:
    Store(QObject *parent);
    QStandardItemModel* model;

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
        this->model->clear();
    }

public slots:
    virtual void loadTranslationTable() {
        // Implement this if needed
    }
};

#endif // STORE_H
