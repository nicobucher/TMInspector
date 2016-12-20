#ifndef DUMPSUMMARYPACKET_H
#define DUMPSUMMARYPACKET_H

#include <QList>
#include "sourcepacket.h"

class DumpSummaryPacket : public SourcePacket
{
public:
    DumpSummaryPacket();

    void decode();

    QList<int> getL_sequencecounts() const;

protected:
    QList<int> l_sequencecounts;
    int dumpcounter;
    int dumpid;
};

#endif // DUMPSUMMARYPACKET_H
