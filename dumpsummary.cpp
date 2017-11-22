#include "dumpsummary.h"

DumpSummary::DumpSummary(QObject* parent, DumpSummaryPacket* init_packet) : QObject(parent)
{
    this->object_id = init_packet->getOnboardStoreObject_id();
    this->object_name = init_packet->getObject_name();
    this->uniqueId = init_packet->generateUniqueId();
    this->dumpId = init_packet->getDumpid();
    this->reception_time = init_packet->getReceptionTime();
    putDumpSummaryPacket(init_packet);
}

void DumpSummary::putDumpSummaryPacket(DumpSummaryPacket *pkt_)
{
    this->l_summary_packets.insert(pkt_->getDumpcounter(), pkt_);
    add_found_packets(pkt_->getL_found_packets());
}

QDateTime DumpSummary::getReception_time() const
{
    return reception_time;
}

bool DumpSummary::isFresh()
{
    if (this->reception_time.secsTo(QDateTime::currentDateTime()) > 3600) {
        return false;
    } else {
        return true;
    }
}

void DumpSummary::addMissingCounts(QHash<uint16_t, uint16_t> counts_)
{
    this->l_missingcounts.unite(counts_);
}

QHash<uint16_t, uint16_t> *DumpSummary::getMissingCounts()
{
    return &l_missingcounts;
}

uint32_t DumpSummary::getObject_id() const
{
    return object_id;
}

QString DumpSummary::getObject_name() const
{
    return object_name;
}

void DumpSummary::setObject_name(const QString &value)
{
    object_name = value;
}

uint8_t DumpSummary::getDumpId() const
{
    return dumpId;
}

void DumpSummary::setDumpId(const uint8_t &value)
{
    dumpId = value;
}

qulonglong DumpSummary::getUniqueId() const
{
    return uniqueId;
}

void DumpSummary::setUniqueId(const qulonglong &value)
{
    uniqueId = value;
}

QList<SourcePacket *> DumpSummary::getL_found_packets() const
{
    return l_found_packets;
}

void DumpSummary::add_found_packets(const QList<SourcePacket *> &value)
{
    l_found_packets.append(value);
}
