#include "dumpmodel.h"

DumpModel::DumpModel(DumpStore *parentStore_) :
    parentStore(parentStore_)
{

}

DumpModel &DumpModel::operator <<(DumpSummary *dump_summary_)
{
    int row = checkDumpExists(dump_summary_->getDump_id());

    if (row < 0) {
        // New Dump
    } else {
        this->removeRow(row);
    }
    QStandardItem* new_dump = new QStandardItem(dump_summary_->getObject_name() + " (No. " + QString::number(dump_summary_->getDump_id()) + ")");
    new_dump->setData(dump_summary_->getDump_id(), IdentifierRole);

    QHashIterator<uint16_t, DumpSummaryPacket*> it(dump_summary_->getSummaryPackets());
    while (it.hasNext()) {
        it.next();
        appendSummaryPacket(new_dump, it.value());
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
    new_summary_packet_->setData(dump_summary_packet_->getId(), ListIndexRole);
    if (dump_summary_packet_->isComplete()) {
        new_summary_packet_->setData(QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
    }
    QHashIterator<uint16_t, uint16_t> it(dump_summary_packet_->getL_sequencecounts());
    while (it.hasNext()) {
        it.next();
        QStandardItem* new_packet_ = new QStandardItem(QString::number(it.key()) + " (APID: " + QString::number(it.value()) + ")");
        new_summary_packet_->appendRow(new_packet_);
    }
    it = QHashIterator<uint16_t, uint16_t>(dump_summary_packet_->getL_missing_sequencecounts());
    while (it.hasNext()) {
        it.next();
        QStandardItem* new_packet_ = new QStandardItem(QString::number(it.key()) + " (APID: " + QString::number(it.value()) + ")");
        new_packet_->setData(QVariant(QBrush(Qt::red)), Qt::ForegroundRole);
        new_summary_packet_->appendRow(new_packet_);
    }
    item_->appendRow(new_summary_packet_);
}

int DumpModel::checkDumpExists(uint8_t dumpId_)
{
    QStandardItem *root = this->invisibleRootItem();
    for (int i = 0; i < root->rowCount(); i++) {
        int test_id = root->child(i)->data(IdentifierRole).toInt();
        if (test_id == dumpId_) return i;
    }
    return -1;
}

void DumpModel::clear()
{
    this->clear();
}

void DumpModel::setMyTimestampFmt(const QString &value)
{
    myTimestampFmt = value;
}
