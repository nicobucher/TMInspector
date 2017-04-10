#ifndef OBJECTFILTERPROXYMODEL_H
#define OBJECTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class ObjectFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ObjectFilterProxyModel(QObject *parent = 0, QModelIndex selectedItemIndex_ = QModelIndex());

protected:
    QModelIndex selectedItemIndex;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
};

#endif // OBJECTFILTERPROXYMODEL_H
