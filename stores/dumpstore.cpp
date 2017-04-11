#include "dumpstore.h"
#include "mainwindow.h"

DumpStore::DumpStore(QObject *parent) :
    Store(parent)
{
    MainWindow* mainwindow = (MainWindow*)parent;

    this->model = new DumpModel(this, mainwindow->settings->value("time_fmt").toString());
    this->proxy_model = new QSortFilterProxyModel(this);
    this->setSourceModel(this->model);
}

bool DumpStore::itemInStore(QString obj_id)
{
    // TODO
    return false;
}

int DumpStore::getNumberOfItems()
{
    // TODO
    return 0;
}

void DumpStore::emptyStore()
{
    this->model->clear();
    this->l_summaries.clear();
}

QStandardItemModel *DumpStore::getModel()
{
    return this->model;
}

void DumpStore::putDumpSummaryPacket(DumpSummaryPacket *dps_)
{
    // TODO This should be moved here, was formerly in PacketWorker, should be commented out once stores are global
//    QHash<uint16_t, uint16_t> missingCounts = checkSequenceCounts(ds_packet->getL_sequencecounts());

//    ds_packet->setL_missing_sequencecounts(missingCounts);
//    DumpSummary* summary = dump_store->getDumpSummary(ds_packet->getDumpid(), ds_packet->getOnboardStoreObject_id());
//    summary->addMissingCounts(missingCounts);

    DumpSummary* dump_summary_;

    qulonglong id_ = generateId(dps_->getOnboardStoreObject_id(), dps_->getDumpid());
    if (!this->containsDumpId(id_)) {
        dump_summary_ = new DumpSummary(this, dps_);
        l_summaries.insert(id_, dump_summary_);
        *this->model << dump_summary_;
    } else {
        dump_summary_ = this->getDumpSummary(id_);
        if (dump_summary_->isFresh()) {
            dump_summary_->putDumpSummaryPacket(dps_);
            *this->model << dps_;
        } else {
            // The found Dump Summary is too old: create a new one
            l_summaries.remove(id_);
            dump_summary_ = new DumpSummary(this, dps_);
            l_summaries.insert(id_, dump_summary_);
            *this->model << dump_summary_;
        }
    }
}

void DumpStore::exportToFile(QString filename_)
{
    // TODO
}

DumpSummary *DumpStore::getDumpSummary(qulonglong id_)
{
    return this->l_summaries.value(id_);
}

DumpSummary *DumpStore::getDumpSummary(uint8_t dump_id, uint32_t object_id)
{
    return this->l_summaries.value(generateId(object_id, dump_id));
}

bool DumpStore::containsDumpId(uint32_t obj_id_, uint8_t dump_id_) {
    return this->l_summaries.contains(generateId(obj_id_, dump_id_));
}

bool DumpStore::containsDumpId(qulonglong id_) {
    return this->l_summaries.contains(id_);
}

int DumpStore::getNumberOfDumps() {
    return this->l_summaries.size();
}
