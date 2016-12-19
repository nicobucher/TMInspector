#ifndef DUMPSUMMARYPACKET_H
#define DUMPSUMMARYPACKET_H

#include <QList>
#include "sourcepacket.h"

class DumpSummaryPacket : public SourcePacket
{
public:
    DumpSummaryPacket(SourcePacket* packet);

protected:
    QList<int> apids;
};

#endif // DUMPSUMMARYPACKET_H
