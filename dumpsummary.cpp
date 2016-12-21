#include "dumpsummary.h"

DumpSummary::DumpSummary(QObject* parent, DumpSummaryPacket* init_packet)
{
    this->object_id = init_packet->getOnboardStoreObject_id();
    this->id = init_packet->getDumpid();
    this->reception_time = QDateTime::currentDateTime();
    putDumpSummaryPacket(init_packet);
}

void DumpSummary::putDumpSummaryPacket(DumpSummaryPacket *pkt_)
{
    this->l_summary_packets.insert(pkt_->getDumpcounter(), pkt_);
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
