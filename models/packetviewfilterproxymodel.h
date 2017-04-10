#ifndef PACKETVIEWFILTERPROXYMODEL_H
#define PACKETVIEWFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class PacketViewFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    PacketViewFilterProxyModel(QObject* parent = 0);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
};

#endif // PACKETVIEWFILTERPROXYMODEL_H
