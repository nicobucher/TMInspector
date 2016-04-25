#include "packetviewfilterproxymodel.h"

PacketViewFilterProxyModel::PacketViewFilterProxyModel(QObject *parent)
: QSortFilterProxyModel(parent)
{
    // Initialize anything?
}

bool PacketViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index_type = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex index_subtype = sourceModel()->index(source_row, 2, source_parent);

    QStringList types = filterRegExp().pattern().split(",");

    return true;
}

bool PacketViewFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{

}
