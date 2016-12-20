#include "dumpsummarypacket.h"

DumpSummaryPacket::DumpSummaryPacket()
{
    // Todo: Parse the packet and extract the list of ssc's
}

void DumpSummaryPacket::decode()
{
    // Extract the dump id and dump counter
    this->dumpid = (this->data[0] << 8) + this->data[1];
    this->dumpcounter = (this->data[2] << 8) + this->data[3];

    int new_ssc_;
    int i = 4;
    while(i < this->dataLength) {
        new_ssc_ = (this->data[i] << 24) + (this->data[i+1] << 16) + (this->data[i+2] << 8) + this->data[i+3];
        this->l_sequencecounts.append(new_ssc_);
        i = i+4;
    }
}

QList<int> DumpSummaryPacket::getL_sequencecounts() const
{
    return l_sequencecounts;
}
