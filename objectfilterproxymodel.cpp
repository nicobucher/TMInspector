#include "objectfilterproxymodel.h"

ObjectFilterProxyModel::ObjectFilterProxyModel(QObject *parent, QModelIndex selectedItemIndex_)
    : QSortFilterProxyModel(parent), selectedItemIndex(selectedItemIndex_)
{

}

bool
ObjectFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);
    if (index1.parent() == selectedItemIndex || index0 == selectedItemIndex) {
        return true;
    } else {
        return false;
    }
}

bool
ObjectFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
//    if (source_column == 1) {
//        return true;
//    } else {
//        return false;
//    }
}

