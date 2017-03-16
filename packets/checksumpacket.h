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

    uint32_t getObject_id() const;
    uint32_t getAddress() const;
    uint16_t getCheck_length() const;
    uint16_t getChecksum() const;

protected:
    uint32_t object_id;
    uint32_t address;
    uint16_t check_length;
    uint16_t checksum;
};

#endif // CHECKSUMPACKET_H
