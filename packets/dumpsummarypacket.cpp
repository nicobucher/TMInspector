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
    this->dumpid = p_[4];
    this->dumpcounter = (p_[5] << 8) + p_[6];
    uint8_t n = p_[7];

    int ssc_apid_ = 0;
    int pos = 7 + n*4;
    if (pos > this->dataLength) {
        // Wrong number of entries
        return;
    }
    while(pos > 7) {
        ssc_apid_ = (p_[pos-3] << 24) + (p_[pos-2] << 16) + (p_[pos-1] << 8) + p_[pos];
        this->l_sequencecounts.insert(ssc_apid_, false);
        pos = pos - 4;
    }
}

QHash<uint32_t, bool> DumpSummaryPacket::getL_sequencecounts() const
{
    return l_sequencecounts;
}

uint32_t DumpSummaryPacket::getOnboardStoreObject_id() const
{
    return object_id;
}

uint16_t DumpSummaryPacket::getDumpcounter() const
{
    return dumpcounter;
}

uint8_t DumpSummaryPacket::getDumpid() const
{
    return dumpid;
}

bool DumpSummaryPacket::isComplete() const
{
    if (l_sequencecounts.size() == l_found_packets.size())
        return true;
    else {
        return false;
    }
}

int DumpSummaryPacket::getNumberOfMissingSSC()
{
    return l_sequencecounts.size() - l_found_packets.size();
}

int DumpSummaryPacket::getNumberOfSSC()
{
    return l_sequencecounts.size();
}

int DumpSummaryPacket::getNumberOfFoundSSC()
{
    return l_found_packets.size();
}

QString DumpSummaryPacket::getObject_name() const
{
    return object_name;
}

qulonglong DumpSummaryPacket::generateUniqueId()
{
    return ((qulonglong)this->object_id << 32) + this->dumpid;
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
        add_found_packets(this->storePointer->checkSequenceCounts(this->l_sequencecounts));
    }
}
