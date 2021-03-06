#include "dumpviewfilterproxymodel.h"
#include "definitions.h"
#include "helpers/variantptr.h"
#include "packets/dumpsummarypacket.h"
#include "stores/dumpstore.h"

DumpViewFilterProxyModel::DumpViewFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool
DumpViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 1, source_parent);
    return (acceptList.contains(index.data(ListIndexRole).toULongLong()));
}

bool
DumpViewFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
}

void DumpViewFilterProxyModel::setFilterIndex(QModelIndex newindex)
{
    QObject* object = VariantPtr<QObject>::asPtr(myDumpStore.getModel()->data(newindex, ObjectRole));
    DumpSummaryPacket* dpsp_ = qobject_cast<DumpSummaryPacket*>(object);
    if (dpsp_ != NULL) {
        acceptList = dpsp_->getL_uniqueIds();
    }
    DumpSummary* dps_ = qobject_cast<DumpSummary*>(object);
    if (dps_ != NULL) {
        acceptList = dps_->getL_packets();
    }
    this->invalidateFilter();
    return;
}
