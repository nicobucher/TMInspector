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
    static const int DAYS_CCSDS_TO_UNIX_EPOCH = 4383;
    static const int SECONDS_PER_DAY = 86400;
    TMSourcePacketDataFieldHeader();
private:
    int version;
    int packetSubCounter;
    QDateTime timestamp;
    bool tsval;
    int coreId;
    int bootCount;

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

    int getCoreId() {
        return this->coreId;
    }

    void setCoreId(int value_) {
        this->coreId = value_;
    }

    int getBootCount() {
        return this->bootCount;
    }

    void setBootCount(int value_) {
        this->bootCount = value_;
    }

    QDateTime getTimestamp() {
        return this->timestamp;
    }

    int getTimestampSeconds() {
        return this->timestamp.toMSecsSinceEpoch()/1000;
    }

    QString getTimestampString(QString fmt_) {
        return this->timestamp.toString(fmt_);
    }

    void setTimestamp(QDateTime ts_) {
        this->timestamp = ts_;
    }

    void setTimestampValid(bool tval_) {
        this->tsval = tval_;
    }

    bool timestampValid() {
        return this->tsval;
    }

    int getLength() {
        return this->length;
    }

    SourcePacketDataFieldHeader* makeDataFieldHeaderFromData(unsigned char* pData_);
    QDateTime makeUTCTimestamp(unsigned long *ts_field_);
    QDateTime decodeFromCDS(uint8_t pField, unsigned long *ts_field_);
};

#endif // SOURCEPACKETDATAFIELDHEADER_H
