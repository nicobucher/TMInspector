#include "sourcepacket.h"
#include "sourcepacketdatafieldheader.h"
#include <QDebug>

SourcePacket::SourcePacket() :
    sequence(STANDALONE_PACKET),
    quality(GOOD)
{
    //
    this->sourceSequenceCount = 0;
    this->setDataField(NULL, 0);
}

SourcePacket::SourcePacket(SourcePacket &packet) :
    QObject(),
    QStandardItem(),
    sequence(STANDALONE_PACKET),
    quality(GOOD),
    version(packet.getVersion()),
    SourcePacketType(packet.getSourcePacketType()),
    sourceSequenceCount(packet.getSourceSequenceCount()),
    apid(packet.getApid()),
    spid(packet.getSpid()),
    unique_id(packet.getId())
{
    this->setDataField((unsigned char*)packet.getData().data(), packet.getDataLength());

    this->dataFieldHeader = new TMSourcePacketDataFieldHeader();
    this->dataFieldHeader->setServiceType(packet.getDataFieldHeader()->getServiceType());
    this->dataFieldHeader->setSubServiceType(packet.getDataFieldHeader()->getSubServiceType());
    this->dataFieldHeader->setTimestamp(packet.getDataFieldHeader()->getTimestamp());
    this->dataFieldHeader->setTimestampValid(packet.getDataFieldHeader()->timestampValid());
}

SourcePacket::SourcePacket(int type_, int version_, int apid_) :
    sequence(STANDALONE_PACKET),
    quality(GOOD)
{
    // Initialization Constructor
    this->sourceSequenceCount = 0;
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

    int dataFieldHeaderPresent = (part1 >> 11) & 0x1;

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

    this->setSourceSequenceCount(part2 & 0x3fff);

    if (dataFieldHeaderPresent != 0) {
        // Set datafield header
        dataFieldHeader = new TMSourcePacketDataFieldHeader();
        dataFieldHeader->makeDataFieldHeaderFromData(pData_);
        qulonglong id_ = ((qulonglong)dataFieldHeader->getTimestampSeconds() << 32) + this->getSourceSequenceCount();
        this->setId(id_);
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

int
SourcePacket::makeSPID(QHash<int, QVariant> *PI_hash_)
{
    if (this->hasDataFieldHeader()) {
        PI_VALUES value_;
        for (QHash<int, QVariant>::iterator it = PI_hash_->begin(); it != PI_hash_->end(); ++it) {
            value_ = it.value().value<PI_VALUES>();
            if (value_.type_key == this->dataFieldHeader->getTypeKey()) {
                if (pi_vals.PI1_VAL == value_.PI1_VAL && pi_vals.PI2_VAL == value_.PI2_VAL)
                    return this->spid = it.key();
            }
        }
    }
    return this->spid = -1;
}

void
SourcePacket::makePI_VALUES(QHash<int, QVariant>* PIC_hash_)
{
    this->pi_vals.PI1_VAL = 0;
    this->pi_vals.PI2_VAL = 0;
    if (!this->hasDataFieldHeader()) {
        return;
    }

    int lookup_key_ = (this->dataFieldHeader->getServiceType() << 16) + this->dataFieldHeader->getSubServiceType();

    QHash<int, QVariant>::iterator it = PIC_hash_->find(lookup_key_);
    if (it == PIC_hash_->end()) {
        return;
    }

    PIC_VALUES value_ = it.value().value<PIC_VALUES>();

    int byte_width = 0;
    int byte_offset = 0;
    unsigned char* pos_;
    // Try to read PI1
    byte_offset = value_.PI1_offset;
    if(byte_offset != -1) {
        byte_width = value_.PI1_width/8;
        pos_ = this->data + byte_offset - 6;
        for (int i = 0; i<byte_width; ++i) {
            this->pi_vals.PI1_VAL = this->pi_vals.PI1_VAL + (pos_[byte_width-1-i] << 8*i);
        }
    } else {
        this->pi_vals.PI1_VAL = 0;
    }
    // Try to read PI2
    byte_offset = value_.PI2_offset;
    if(byte_offset != -1) {
        byte_width = value_.PI2_width/8;
        pos_ = this->data + byte_offset - 6;
        for (int i = 0; i<byte_width; ++i) {
            this->pi_vals.PI2_VAL = this->pi_vals.PI2_VAL + (pos_[byte_width-1-i] << 8*i);
        }
    } else {
        this->pi_vals.PI2_VAL = 0;
    }

    return;
}
