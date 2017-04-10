#include "dumpmodel.h"

DumpModel::DumpModel(DumpStore *parentStore_, QString time_fmt) :
    parentStore(parentStore_),
    myTimestampFmt(time_fmt)
{

}

DumpModel &DumpModel::operator <<(DumpSummary *sum_)
{
    QStandardItem *root = invisibleRootItem();

    QList<QStandardItem*> row;
    QStandardItem* object_id = new QStandardItem(sum_->getObject_id());
    row << object_id;
    row << new QStandardItem(sum_->getReception_time().toString(myTimestampFmt));

    root->appendRow(row);

    return *this;
}
