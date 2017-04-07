#include "dumpstore.h"

DumpStore::DumpStore(QObject *parent) :
    Store(parent)
{
    this->model = new QStandardItemModel(this);
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

int DumpStore::getNumberOfDumps() {
    return this->l_dumps.size();
}

bool DumpStore::containsDumpId(uint32_t obj_id_, uint8_t dump_id_) {
    return this->l_dumps.contains(generateId(obj_id_, dump_id_));
}

DumpSummary *DumpStore::getDumpSummary(qulonglong id_)
{
    return this->l_dumps.value(id_);
}

DumpSummary *DumpStore::getDumpSummary(uint8_t dump_id, uint32_t object_id)
{
    return this->l_dumps.value(generateId(object_id, dump_id));
}

bool DumpStore::containsDumpId(qulonglong id_) {
    return this->l_dumps.contains(id_);
}

qulonglong DumpStore::generateId(uint32_t obj_id_, uint8_t dump_id_) {
    return ((qulonglong)obj_id_ << 32) + dump_id_;
}

void DumpStore::emptyStore()
{
    this->l_dumps.clear();
}

void DumpStore::putDumpSummaryPacket(DumpSummaryPacket *dps_)
{
    DumpSummary* dump_summary_;
    uint8_t dump_id_ = dps_->getDumpid();
    uint32_t object_id_ = dps_->getOnboardStoreObject_id();
    qulonglong id_ = generateId(object_id_, dump_id_);
    if (!this->containsDumpId(id_)) {
        dump_summary_ = new DumpSummary(this, dps_);
        this->l_dumps.insert(id_, dump_summary_);
    } else {
        dump_summary_ = this->l_dumps.value(id_);
        if (dump_summary_->isFresh()) {
            dump_summary_->putDumpSummaryPacket(dps_);
        } else {
            // The found Dump Summary is too old, delete it and create a new one
            this->l_dumps.remove(id_);
            dump_summary_ = new DumpSummary(this, dps_);
            this->l_dumps.insert(id_, dump_summary_);
        }
    }
}

void DumpStore::exportToFile(QString filename_)
{
    // TODO
}
