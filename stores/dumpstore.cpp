#include "dumpstore.h"
#include "mainwindow.h"
#include <QSettings>

extern QSettings settings;

DumpStore::DumpStore(QObject *parent) :
    Store(parent)
{
    this->model = new DumpModel(this);
    this->proxy_model = new DumpViewFilterProxyModel(this);
}

void DumpStore::setModelTimestampFmt(const QString &value) {
    this->model->setMyTimestampFmt(value);
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

void DumpStore::putDumpSummaryPacket(SourcePacket *packet_)
{
    qDebug() << "DumpStore::Putting DumpSummary " << packet_->getSourceSequenceCount();
    DumpSummary* dump_summary_;
    DumpSummaryPacket* dps_ = (DumpSummaryPacket*)packet_;

    dps_->checkPackets();

    qulonglong id_ = dps_->generateUniqueId();
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

bool DumpStore::containsDumpId(qulonglong id_) {
    return this->l_summaries.contains(id_);
}

int DumpStore::getNumberOfDumps() {
    return this->l_summaries.size();
}
