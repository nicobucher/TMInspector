#include "dumpsummarypacket.h"

DumpSummaryPacket::DumpSummaryPacket(SourcePacket &packet) : SourcePacket(packet)
{
    decode();
}

void DumpSummaryPacket::decode()
{
    unsigned char* p_ = &this->data[12];
    this->object_id = (p_[0] << 24) + (p_[1] << 16) + (p_[2] << 8) + p_[3];
    // Extract the dump id and dump counter
    this->dumpid = p_[4];
    this->dumpcounter = (p_[5] << 8) + p_[6];
    uint8_t n = p_[7];

    int new_ssc_ = 0;
    int ssc_apid_ = 0;
    int pos = 7 + n*4;
    if (pos > this->dataLength) {
        // Wrong number of entries
        return;
    }
    while(pos > 7) {
        pos = pos - 8;
        ssc_apid_ = (p_[pos-3] << 8) + p_[pos-2];
        new_ssc_ = (p_[pos-1] << 8) + p_[pos];
        this->l_sequencecounts.insert(new_ssc_, ssc_apid_);
    }
}

QHash<uint16_t, uint16_t> DumpSummaryPacket::getL_sequencecounts() const
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
    if (l_missing_sequencecounts.size() == 0)
        return true;
    else {
        return false;
    }
}

QHash<uint16_t, uint16_t> DumpSummaryPacket::getL_missing_sequencecounts() const
{
    return l_missing_sequencecounts;
}

void DumpSummaryPacket::setL_missing_sequencecounts(const QHash<uint16_t, uint16_t> &value)
{
    l_missing_sequencecounts = value;
}

int DumpSummaryPacket::getNumberOfMissingSSC()
{
    return l_missing_sequencecounts.size();
}
