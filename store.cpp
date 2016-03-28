#include "store.h"

Store::Store(QObject* parent) : QObject(parent)
{
    this->model = new QStandardItemModel();
    this->proxy_model = new TreeViewFilterProxyModel(this);
    this->proxy_model->setSourceModel(this->model);
}
