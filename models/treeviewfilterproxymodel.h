#ifndef TREEVIEWFILTERPROXYMODEL_H
#define TREEVIEWFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TreeViewFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TreeViewFilterProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
};

#endif // TREEVIEWFILTERPROXYMODEL_H
