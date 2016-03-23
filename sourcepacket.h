#ifndef SOURCEPACKET_H
#define SOURCEPACKET_H

#include "sourcepacketdatafieldheader.h"
#include <QObject>

enum Quality {
    GOOD,
    BAD
};

enum Sequence{
    CONT_PACKET = 0x00,
    FIRST_PACKET,
    LAST_PACKET,
    STANDALONE_PACKET
};

class SourcePacket : public QObject
{
    Q_OBJECT
public:
    SourcePacket();
    SourcePacket(int type_, int version_, int apid_);
    ~SourcePacket();

    static const int APID_TIMEPACKET = 0x000;
    static const int APID_IDLEPACKET = 0x7FF;
    static const int HEADER_SIZE = 6;

private:
    int version;
    int SourcePacketType;
    int apid;
    Sequence sequence;
    int sourceSequenceCount;
    bool dataFieldHeaderIsPresent;
    SourcePacketDataFieldHeader* dataFieldHeader;
    Quality quality;
    unsigned char* header;
    unsigned char* data;
    int dataLength;

public:
    int getVersion() {
        return this->version;
    }

    void setVersion(int v_) {
        this->version = v_;
    }

    int getSourcePacketType() {
        return this->SourcePacketType;
    }

    void setSourcePacketType(int spt_) {
        this->SourcePacketType = spt_;
    }

    int getApid() {
        return this->apid;
    }

    void setApid(int a_) {
        this->apid = a_;
    }

    Sequence getSequence() {
        return this->sequence;
    }

    void setSequence(Sequence sq_) {
        this->sequence = sq_;
    }

    int getSourceSequenceCount() {
        return this->sourceSequenceCount;
    }

    void setSourceSequenceCount(int ssc_) {
       this->sourceSequenceCount = ssc_;
    }

    Quality getQuality() {
        return this->quality;
    }

    void setQuality(Quality q_) {
        this->quality= q_;
    }

    bool hasDataFieldHeader() {
        if (this->dataFieldHeaderIsPresent != 0) {
            return true;
        } else {
            return false;
        }
    }

    void setDataFieldHeaderIsPresent(int present_) {
        this->dataFieldHeaderIsPresent = present_;
    }

    SourcePacketDataFieldHeader* getDataFieldHeader() {
        return this->dataFieldHeader;
    }

    void setDataFieldHeader(SourcePacketDataFieldHeader* hdr_) {
        this->dataFieldHeader = hdr_;
    }

    static int getApidTimepacket() {
        return APID_TIMEPACKET;
    }

    static int getApidIdlePacket() {
        return APID_IDLEPACKET;
    }

    QByteArray getData() {
        return QByteArray((const char*)this->data);
    }

    void setData(unsigned char* d_, int length_) {
        this->data = (unsigned char*)malloc(length_);
        memcpy(this->data, d_, length_);
    }

    void setHeader(unsigned char* d_) {
        this->header = (unsigned char*)malloc(HEADER_SIZE);
        memcpy(this->header, d_, HEADER_SIZE);
    }

    int getDataLength() {
        return dataLength-1;
    }

    bool checkCRC();

    SourcePacket* makePacketFromData(unsigned char* pHeader, unsigned char* pData_, int length_);
};

#endif // SOURCEPACKET_H
