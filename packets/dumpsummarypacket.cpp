#include "dumpsummarypacket.h"
#include "translator.h"
#include "stores/packetstore.h"

DumpSummaryPacket::DumpSummaryPacket(SourcePacket &packet) : SourcePacket(packet)
{
    decode();
}

void DumpSummaryPacket::decode()
{
    unsigned char* p_ = &this->data[12];
    this->object_id = (p_[0] << 24) + (p_[1] << 16) + (p_[2] << 8) + p_[3];
    QVariant obj_name_ = myObjectTranslator.translate(this->object_id);
    if (obj_name_.isValid()) {
        this->object_name = obj_name_.toString();
    } else {
        this->object_name = QString::number(this->object_id);
    }
    // Extract the dump id and dump counter
    this->dumpid = (this->object_id << 32) + p_[4];
    this->dumpcounter = (p_[5] << 8) + p_[6];
    uint8_t n = p_[7];

    qulonglong unique_id = 0;
    int pos = 7 + n*8;
    if (pos > this->dataLength) {
        // Wrong number of entries
        return;
    }
    while(pos > 7) {
        unique_id =
                (p_[pos-7] << 56) + (p_[pos-6] << 48) +
                (p_[pos-5] << 40) + (p_[pos-4] << 32) +
                (p_[pos-3] << 24) + (p_[pos-2] << 16) +
                (p_[pos-1] << 8) + p_[pos];
        this->l_unique_ids.insert(unique_id, false);
        pos = pos - 8;
    }
}

QHash<qulonglong, bool> DumpSummaryPacket::getL_uniqueIds() const
{
    return l_unique_ids;
}

uint32_t DumpSummaryPacket::getOnboardStoreObject_id() const
{
    return object_id;
}

uint16_t DumpSummaryPacket::getDumpcounter() const
{
    return dumpcounter;
}

qulonglong DumpSummaryPacket::getDumpid() const
{
    return dumpid;
}

bool DumpSummaryPacket::isComplete() const
{
    if (l_unique_ids.size() == l_found_packets.size())
        return true;
    else {
        return false;
    }
}

int DumpSummaryPacket::getNumberOfMissingSSC()
{
    return l_unique_ids.size() - l_found_packets.size();
}

int DumpSummaryPacket::getNumberOfSSC()
{
    return l_unique_ids.size();
}

int DumpSummaryPacket::getNumberOfFoundSSC()
{
    return l_found_packets.size();
}

QString DumpSummaryPacket::getObject_name() const
{
    return object_name;
}

QList<SourcePacket *> DumpSummaryPacket::getL_found_packets() const
{
    return l_found_packets;
}

void DumpSummaryPacket::add_found_packets(const QList<SourcePacket *> &value)
{
    l_found_packets.append(value);
}

void DumpSummaryPacket::checkPackets()
{
    if(this->storePointer != NULL) {
        add_found_packets(this->storePointer->checkUniqueIds(this->l_unique_ids));
    }
}
