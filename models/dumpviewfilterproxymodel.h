#ifndef DUMPVIEWFILTERPROXYMODEL_H
#define DUMPVIEWFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "packets/sourcepacket.h"

class DumpViewFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DumpViewFilterProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

public:
    void setFilterIndex(QModelIndex newindex);

private:
    QHash<qulonglong, bool> acceptList;
};

#endif // DUMPVIEWFILTERPROXYMODEL_H
