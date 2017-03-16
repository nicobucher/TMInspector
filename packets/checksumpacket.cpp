#include "checksumpacket.h"

ChecksumPacket::ChecksumPacket(SourcePacket &packet) : SourcePacket(packet)
{
    decode();
}

void ChecksumPacket::decode()
{
    unsigned char* p_ = &this->data[12];
    this->object_id = (p_[0] << 24) + (p_[1] << 16) + (p_[2] << 8) + p_[3];
    // Extract the dump id and dump counter
    this->address = (p_[4] << 24) + (p_[5] << 16) + (p_[6] << 8) + p_[7];
    this->check_length = (p_[8] << 8) + p_[9];
    this->checksum = (p_[10] << 8) + p_[11];
}

uint32_t ChecksumPacket::getObject_id() const
{
    return object_id;
}

uint32_t ChecksumPacket::getAddress() const
{
    return address;
}

uint16_t ChecksumPacket::getCheck_length() const
{
    return check_length;
}

uint16_t ChecksumPacket::getChecksum() const
{
    return checksum;
}
