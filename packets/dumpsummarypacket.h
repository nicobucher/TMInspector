#ifndef DUMPSUMMARYPACKET_H
#define DUMPSUMMARYPACKET_H

#include <QHash>
#include "sourcepacket.h"

class DumpSummaryPacket : public SourcePacket
{
public:
    DumpSummaryPacket();

    void decode();

    QHash<uint16_t, uint16_t> getL_sequencecounts() const;

    uint32_t getOnboardStoreObject_id() const;
    uint16_t getDumpcounter() const;
    uint8_t getDumpid() const;

protected:
    QHash<uint16_t, uint16_t> l_sequencecounts;
    uint32_t object_id;
    uint16_t dumpcounter;
    uint8_t dumpid;
};

#endif // DUMPSUMMARYPACKET_H
