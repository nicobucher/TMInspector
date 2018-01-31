#ifndef SOURCEPACKET_H
#define SOURCEPACKET_H

#include "sourcepacketdatafieldheader.h"
#include "definitions.h"
#include <QStandardItem>
#include <QObject>

class PacketStore;

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

class SourcePacket : public QObject, public QStandardItem
{
    Q_OBJECT
public:
    SourcePacket();
    SourcePacket(SourcePacket &packet);
    SourcePacket(int type_, int version_, int apid_);
    ~SourcePacket();

    static const int APID_TIMEPACKET = 0x000;
    static const int APID_IDLEPACKET = 0x7FF;
    static const int HEADER_SIZE = 6;
    static const int MAX_PACKET_SIZE = 2048;

protected:
    Sequence sequence;
    Quality quality;
    int version;
    int SourcePacketType;
    int sourceSequenceCount;
    int apid;
    int spid;
    PI_VALUES pi_vals;
    TMSourcePacketDataFieldHeader* dataFieldHeader;
    unsigned char* header;
    unsigned char* data;
    int dataLength;
    QString mibVersion;
    QString name;
    QDateTime receptionTime;
    qulonglong unique_id;

    PacketStore* storePointer;

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

    int getSpid() {
        return this->spid;
    }

    void setSpid(int s_) {
        this->spid = s_;
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
        if (this->dataFieldHeader != NULL) {
            return true;
        } else {
            return false;
        }
    }

    TMSourcePacketDataFieldHeader* getDataFieldHeader() {
        return this->dataFieldHeader;
    }

    void setDataFieldHeader(TMSourcePacketDataFieldHeader* hdr_) {
        this->dataFieldHeader = hdr_;
    }

    static int getApidTimepacket() {
        return APID_TIMEPACKET;
    }

    static int getApidIdlePacket() {
        return APID_IDLEPACKET;
    }

    QByteArray getData() {
        return QByteArray((const char*)this->data, this->dataLength);
    }

    void setDataField(unsigned char* d_, int length_) {
        this->dataLength = length_;
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

    void setPI_1(int pi_) {
        this->pi_vals.PI1_VAL = pi_;
    }

    void setPI_2(int pi_) {
        this->pi_vals.PI2_VAL = pi_;
    }

    int getPI_1() {
        return this->pi_vals.PI1_VAL;
    }

    int getPI_2() {
        return this->pi_vals.PI2_VAL;
    }

    QString getMibVersion() {
        return this->mibVersion;
    }

    void setMibVersion(const QString &value) {
        this->mibVersion = value;
    }

    void setUnique_id(qulonglong id_) {
        this->unique_id = id_;
    }

    bool checkCRC();

    SourcePacket* makePacketFromData(unsigned char* pHeader, unsigned char* pData_, int length_);

    int makeSPID();
    void makePI_VALUES();
    QString getName() const;
    void setName(QString name_);
    PacketStore *getStorePointer() const;
    void setStorePointer(PacketStore *value);
    QDateTime getReceptionTime() const;
    void setReceptionTime(const QDateTime &value);
    qulonglong getUnique_id() const;
    void generateUnique_id();
};

#endif // SOURCEPACKET_H
