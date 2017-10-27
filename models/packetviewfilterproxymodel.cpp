#include "packetviewfilterproxymodel.h"

PacketViewFilterProxyModel::PacketViewFilterProxyModel(QObject *parent)
: QSortFilterProxyModel(parent)
{
    // Initialize anything?
}

bool PacketViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index_type = sourceModel()->index(source_row, 2, source_parent);
    QModelIndex index_subtype = sourceModel()->index(source_row, 3, source_parent);

    if (filterRegExp().pattern() == "*" || filterRegExp().pattern() == "") {
        return true;
    }

    QStringList types = filterRegExp().pattern().split(",");

    if (types.size() == 2) {
        if (types.at(0) == index_type.data(Qt::DisplayRole) &&
            (types.at(1) == index_subtype.data(Qt::DisplayRole) || types.at(1) == "")) {
            return true;
        } else {
            return false;
        }
    }

    for (int i = 0; i < types.size(); ++i) {
        QString str_ = types.at(i);
        if (str_.length() > 1) {
            if (str_[0] == QChar('!')) {
                if (str_.remove(0,1) == index_type.data(Qt::DisplayRole)) {
                    return false;
                }
            }
        }
        if (str_ == index_type.data(Qt::DisplayRole)) {
            return true;
        }
    }
    return false;
}

bool PacketViewFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
}
