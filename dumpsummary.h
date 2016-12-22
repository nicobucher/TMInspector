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

    void addMissingCounts(QHash<uint16_t, uint16_t> counts_);
    QHash<uint16_t, uint16_t>* getMissingCounts();

    uint32_t getObject_id() const;

private:
    /*
     * A dump summary is identified by a combination of onboard object id and
     * dump id.
     */
    uint8_t id;
    uint32_t object_id;
    QHash<uint16_t, DumpSummaryPacket*> l_summary_packets;
    QHash<uint16_t, uint16_t> l_missingcounts;
    QDateTime reception_time;
};

#endif // DUMPSUMMARY_H
