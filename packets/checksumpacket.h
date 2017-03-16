#ifndef CHECKSUMPACKET_H
#define CHECKSUMPACKET_H

#include "sourcepacket.h"

class ChecksumPacket : public SourcePacket
{
public:
    /*
     * Copy Constructor to copy a source packet
     */
    ChecksumPacket(SourcePacket &packet);

    void decode();

    qint32 getObject_id() const;
    qint32 getAddress() const;
    qint16 getCheck_length() const;
    qint16 getChecksum() const;

protected:
    qint32 object_id;
    qint32 address;
    qint32 check_length;
    qint32 checksum;
};

#endif // CHECKSUMPACKET_H
