#include "treeviewfilterproxymodel.h"

TreeViewFilterProxyModel::TreeViewFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool
TreeViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);

    return (sourceModel()->data(index1).toString().contains(filterRegExp())
            || sourceModel()->data(index0).toString().contains(filterRegExp())
            || (index0.parent() == QModelIndex()));
}

bool
TreeViewFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
}

