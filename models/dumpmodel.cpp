#include "dumpmodel.h"
#include "helpers/variantptr.h"
#include "stores/packetstore.h"

DumpModel::DumpModel(DumpStore *parentStore_) :
    parentStore(parentStore_)
{

}

DumpModel &DumpModel::operator <<(DumpSummary *dump_summary_)
{
    int row = checkDumpExists(dump_summary_->getDumpId());

    if (row < 0) {
        // New Dump
    } else {
        this->removeRow(row);
    }
    QStandardItem* new_dump = new QStandardItem(dump_summary_->getObject_name() + " (No. " + QString::number(dump_summary_->getDumpId()) + ")");
    new_dump->setData(VariantPtr<QObject>::asQVariant(dump_summary_), ObjectRole);
    new_dump->setData(dump_summary_->getDumpId(), IdentifierRole);

    PacketStore* store = dump_summary_->getStorePointer();
    new_dump->setData(VariantPtr<PacketStore>::asQVariant(store), StorePointerRole);

    QHashIterator<qulonglong, bool> it(dump_summary_->getSummaryPackets());
    while (it.hasNext()) {
        it.next();
        DumpSummaryPacket* dump_summary_packet = dynamic_cast<DumpSummaryPacket*>(dump_summary_->getStorePointer()->getPacket(it.key()));
        if (dump_summary_packet != NULL) {
            appendSummaryPacket(new_dump, dump_summary_packet);
        } else {
            // No dump summary packet returned, something is wrong!
        }
    }

    this->appendRow(new_dump);
    return *this;
}

DumpModel &DumpModel::operator <<(DumpSummaryPacket* dump_summary_packet_)
{
    QStandardItem *root = invisibleRootItem();
    int row = checkDumpExists(dump_summary_packet_->getDumpid());

    if (row < 0) {
        // No Existing Dump Found, can not be appended
    } else {
        appendSummaryPacket(root->child(row), dump_summary_packet_);
    }

    return *this;
}

void DumpModel::appendSummaryPacket(QStandardItem* item_, DumpSummaryPacket* dump_summary_packet_) {
    QStandardItem* new_summary_packet_ = new QStandardItem("Summary " + QString::number(dump_summary_packet_->getDumpcounter()));
    new_summary_packet_->setData(dump_summary_packet_->getDumpcounter(), IdentifierRole);
    new_summary_packet_->setData(dump_summary_packet_->getUnique_id(), ListIndexRole);
    new_summary_packet_->setData(VariantPtr<PacketStore>::asQVariant(dump_summary_packet_->getStorePointer()), StorePointerRole);
    new_summary_packet_->setData(VariantPtr<QObject>::asQVariant(dump_summary_packet_), ObjectRole);
    if (dump_summary_packet_->isComplete()) {
        new_summary_packet_->setData(QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
    }

    item_->appendRow(new_summary_packet_);
}

int DumpModel::checkDumpExists(qulonglong dumpId_)
{
    QStandardItem *root = this->invisibleRootItem();
    for (int i = 0; i < root->rowCount(); i++) {
        qulonglong test_id = root->child(i)->data(IdentifierRole).toLongLong();
        if (test_id == dumpId_) return i;
    }
    return -1;
}

void DumpModel::setMyTimestampFmt(const QString &value)
{
    myTimestampFmt = value;
}
