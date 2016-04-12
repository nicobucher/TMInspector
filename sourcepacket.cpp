#include "sourcepacket.h"
#include "sourcepacketdatafieldheader.h"
#include <QDebug>

SourcePacket::SourcePacket() : sequence(STANDALONE_PACKET), quality(GOOD)
{
    //
    this->sourceSequenceCount = 0;
    this->dataFieldHeaderIsPresent = false;
    this->setDataField(NULL, 0);
}

SourcePacket::SourcePacket(int type_, int version_, int apid_) : sequence(STANDALONE_PACKET), quality(GOOD)
{
    // Initialization Constructor
    this->sourceSequenceCount = 0;
    this->dataFieldHeaderIsPresent = false;
    this->apid = apid_;
    this->version = version_;
    this->SourcePacketType = type_;

    this->setDataField(NULL, 0);
}

SourcePacket::~SourcePacket()
{
    free(this->data);
}

SourcePacket* SourcePacket::makePacketFromData(unsigned char* pHeader_, unsigned char* pData_, int length_)
{
    // byte 0 & 1
    short part1 = (pHeader_[0] << 8) + pHeader_[1];

    // byte 2 & 3
    short part2 = (pHeader_[2] << 8) + pHeader_[3];

    // byte 4 & 5
    this->setDataField(pData_, length_);
    this->setHeader(pHeader_);

    this->setVersion((part1 >> 13) & 0x7);

    if (this->getVersion() != 0x03) {
        // Packet has CRC checksum

        if (!this->checkCRC()) {
            // Wrong Checksum
            this->setQuality(BAD);
            qDebug() << "SourcePacket::makePacketFromData : Wrong Checksum";
        }
    }

    this->setSourcePacketType((part1 >> 12) & 0x1);
    if (this->getSourcePacketType() != 0) {
        this->setQuality(BAD);
        qDebug() << "SourcePacket::makePacketFromData : Packet Type is not TM (not supported)";
    }

    this->setDataFieldHeaderIsPresent((part1 >> 11) & 0x1);

    this->setApid(part1 & 0x7ff);

    int seqFlags_ = ((part2 >> 14) & 0x03);
    if (seqFlags_ == CONT_PACKET) {
        this->setSequence(CONT_PACKET);
    } else if (seqFlags_ == FIRST_PACKET) {
        this->setSequence(FIRST_PACKET);
    } else if (seqFlags_ == LAST_PACKET) {
        this->setSequence(LAST_PACKET);
    } else if (seqFlags_ == STANDALONE_PACKET) {
        this->setSequence(STANDALONE_PACKET);
    }

    this->setSourceSequenceCount(part2 & 0x3ff);

    if (this->hasDataFieldHeader()) {
        // Set datafield header
        dataFieldHeader = new TMSourcePacketDataFieldHeader();
        dataFieldHeader->makeDataFieldHeaderFromData(pData_);
        // Move data pointer
//        this->dataLength -= 2;
    }

    return this;
}

bool
SourcePacket::checkCRC()
{
    QByteArray complete_packet;
    complete_packet += QByteArray((const char*)this->header, HEADER_SIZE);
    complete_packet += QByteArray((const char*)this->data, this->dataLength);
    int wLength = this->dataLength + this->HEADER_SIZE;

    uint8_t i;
    uint16_t wCrc = 0xffff;

    unsigned char* pData = (unsigned char*)complete_packet.data();

    while (wLength--) {
        wCrc ^= *pData++ << 8;
        for (i=0; i < 8; i++)
            wCrc = wCrc & 0x8000 ? (wCrc << 1) ^ 0x1021 : wCrc << 1;
    }
    if ((wCrc & 0xffff) == 0x00) {
        return true;
    } else {
       return false;
    }
}
