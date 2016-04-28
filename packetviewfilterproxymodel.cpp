#include "packetviewfilterproxymodel.h"

PacketViewFilterProxyModel::PacketViewFilterProxyModel(QObject *parent)
: QSortFilterProxyModel(parent)
{
    // Initialize anything?
}

bool PacketViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index_type = sourceModel()->index(source_row, 1, source_parent);

    if (filterRegExp().pattern() == "*")
        return true;

    QStringList types = filterRegExp().pattern().split(",");

    for (int i = 0; i < types.size(); ++i) {
        if (types.at(i) == index_type.data(Qt::DisplayRole))
            return true;
    }
    return false;
}

bool PacketViewFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{

}
