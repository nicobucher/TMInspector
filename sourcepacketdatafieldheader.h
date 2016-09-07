#ifndef SOURCEPACKETDATAFIELDHEADER_H
#define SOURCEPACKETDATAFIELDHEADER_H

#include <QObject>
#include <QDateTime>

class SourcePacketDataFieldHeader : public QObject
{
    Q_OBJECT
public:
    SourcePacketDataFieldHeader();
protected:
    int serviceType;
    int SubServiceType;
    int type_key;
    int length;

public:
    int getServiceType() {
        return this->serviceType;
    }

    void setServiceType(int type_) {
        this->serviceType = type_;
    }

    int getSubServiceType() {
        return this->SubServiceType;
    }

    void setSubServiceType(int subType_) {
        this->SubServiceType = subType_;
    }

    int getTypeKey() {
        return this->type_key;
    }

    void setTypeKey(int k_) {
        this->type_key = k_;
    }

    virtual QDateTime getTimestamp() =0;
    virtual int getLength() =0;
    virtual SourcePacketDataFieldHeader* makeDataFieldHeaderFromData(unsigned char* data_) =0;
};

class TMSourcePacketDataFieldHeader : public SourcePacketDataFieldHeader
{
    Q_OBJECT
public:
    TMSourcePacketDataFieldHeader();
private:
    int version;
    int packetSubCounter;
    QDateTime timestamp;
public:

    int getVersion(){
        return this->version;
    }

    void setVersion(int v_) {
        this->version = v_;
    }

    int getPacketSubCounter() {
        return this->packetSubCounter;
    }

    void setPacketSubCounter(int subc_) {
        this->packetSubCounter = subc_;
    }

    QDateTime getTimestamp() {
        return this->timestamp;
    }

    void setTimestamp(QDateTime ts_) {
        this->timestamp = ts_;
    }

    int getLength() {
        return this->length;
    }

    SourcePacketDataFieldHeader* makeDataFieldHeaderFromData(unsigned char* pData_);
};

#endif // SOURCEPACKETDATAFIELDHEADER_H
