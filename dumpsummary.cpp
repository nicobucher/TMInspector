#include "dumpsummary.h"
#include "stores/packetstore.h"

DumpSummary::DumpSummary(QObject* parent, DumpSummaryPacket* init_packet) : QObject(parent)
{
    this->setStorePointer(init_packet->getStorePointer());
    this->object_id = init_packet->getOnboardStoreObject_id();
    this->object_name = init_packet->getObject_name();
    this->dumpId = init_packet->getDumpid();
    this->reception_time = init_packet->getReceptionTime();
    putDumpSummaryPacket(init_packet);
}

void DumpSummary::putDumpSummaryPacket(DumpSummaryPacket *pkt_)
{
    this->l_summary_packets.insert(pkt_->getUnique_id(), pkt_);
    PacketStore* store = pkt_->getStorePointer();
    QHashIterator<qulonglong, bool> it(pkt_->getL_uniqueIds());
    SourcePacket* foundPacket = 0;
    while (it.hasNext()) {
        it.next();
        foundPacket = store->getPacket(it.key());
        if(foundPacket != 0) {
            this->add_found_packet(foundPacket);
        }
    }
}

QHash<qulonglong, DumpSummaryPacket*> DumpSummary::getSummaryPackets()
{
    return l_summary_packets;
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

qulonglong DumpSummary::getDumpId() const
{
    return dumpId;
}

void DumpSummary::setDumpId(const qulonglong &value)
{
    dumpId = value;
}

QHash<qulonglong, bool> DumpSummary::getL_packets() const
{
    return this->l_unique_ids;
}

void DumpSummary::add_found_packet(SourcePacket* &value)
{
    this->l_unique_ids.insert(value->getUnique_id(), true);
}

void DumpSummary::add_found_packets(const QHash<qulonglong, bool> &value)
{
    this->l_unique_ids.unite(value);
}

PacketStore *DumpSummary::getStorePointer() const
{
    return storePointer;
}

void DumpSummary::setStorePointer(PacketStore *value)
{
    storePointer = value;
}
