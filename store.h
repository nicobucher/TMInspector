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

    virtual bool itemInStore(QString obj_id) = 0;

    virtual int getNumberOfItems() = 0;

    virtual void emptyStore() = 0;

    virtual QStandardItemModel* getModel() = 0;

    virtual void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

protected:
    TreeViewFilterProxyModel* proxy_model;

    QString export_file_header;

public slots:
    virtual void loadTranslationTable() {
        // Implement this if needed
    }

    virtual void exportToFile(QString filename_) = 0;

    friend class MainWindow;
    friend class ObjectView;
};

#endif // STORE_H
