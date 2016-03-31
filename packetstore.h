#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "sourcepacket.h"

enum Index {
    PacketListIndex = 0,
};

class PacketStore : public Store
{
public:
    PacketStore(QObject *parent);
    void putPacket(SourcePacket* p_);
    SourcePacket* getPacket(int pkt_id);

private:
    QHash<int, SourcePacket*> l_packets;
    int id;
};

#endif // PACKETSTORE_H
