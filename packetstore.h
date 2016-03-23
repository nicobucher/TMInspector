#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "sourcepacket.h"

class PacketStore : public Store
{
public:
    PacketStore(QObject *parent);
    void putPacket(SourcePacket* p_);

private:
};

#endif // PACKETSTORE_H
