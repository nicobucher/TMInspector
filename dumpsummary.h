#ifndef DUMPSUMMARY_H
#define DUMPSUMMARY_H

#include <QObject>
#include "packets/dumpsummarypacket.h"

class DumpSummary : public QObject
{
    Q_OBJECT
public:
    DumpSummary(QObject *parent, DumpSummaryPacket *init_packet);

    void putDumpSummaryPacket(DumpSummaryPacket* pkt_);

    QDateTime getReception_time() const;

    bool isFresh();

    uint32_t getObject_id() const;

private:
    uint8_t id;
    uint32_t object_id;
    QHash<uint16_t, DumpSummaryPacket*> l_summary_packets;
    QDateTime reception_time;
};

#endif // DUMPSUMMARY_H
