#include "dumpsummarypacket.h"

DumpSummaryPacket::DumpSummaryPacket() : complete(false)
{
    // Todo: Parse the packet and extract the list of ssc's
}

void DumpSummaryPacket::decode()
{
    this->object_id = (this->data[0] << 24) + (this->data[1] << 16) + (this->data[2] << 8) + this->data[3];
    // Extract the dump id and dump counter
    this->dumpid = this->data[4];
    this->dumpcounter = (this->data[5] << 8) + this->data[6];
    uint8_t n = this->data[7];

    int new_ssc_ = 0;
    int ssc_apid_ = 0;
    int pos = 7 + n*8;
    while(pos > 7) {
        pos = pos - 8;
        ssc_apid_ = (this->data[pos-7] << 24) + (this->data[pos-6] << 16) + (this->data[pos-5] << 8) + this->data[pos-4];
        new_ssc_ = (this->data[pos-3] << 24) + (this->data[pos-2] << 16) + (this->data[pos-1] << 8) + this->data[pos];
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

bool DumpSummaryPacket::getComplete() const
{
    return complete;
}

void DumpSummaryPacket::setComplete(bool value)
{
    complete = value;
}
