#include "sourcepacketdatafieldheader.h"
#include <QDebug>

SourcePacketDataFieldHeader::SourcePacketDataFieldHeader()
{
    // TODO
}

TMSourcePacketDataFieldHeader::TMSourcePacketDataFieldHeader() : version(1)
{
    this->length = 12;
    this->type_key = 0;
    this->setTimestampValid(false);
}

SourcePacketDataFieldHeader*
TMSourcePacketDataFieldHeader::makeDataFieldHeaderFromData(unsigned char *pData_)
{
    this->setVersion((*pData_ & 0x70) >> 4);
    this->setServiceType(*(pData_+1) & 0xff);
    this->setSubServiceType(*(pData_+2) & 0xff);
    int key_ = (this->getServiceType() << 16) + this->getSubServiceType();
    this->setTypeKey(key_);
    this->setPacketSubCounter(*(pData_+3) & 0xff);

    this->setTimestamp(makeUTCTimestamp((unsigned long*)(pData_+4)));

    return this;
}

QDateTime
TMSourcePacketDataFieldHeader::makeUTCTimestamp(unsigned long *ts_field_)
{
    unsigned char pField = *(unsigned char*)ts_field_;
    unsigned char timeCode = (pField & 0xF0) >> 4;
    //First, check time code part in pField, it tells us how to go on.
    switch (timeCode) {
        case 0b100:
            return decodeFromCDS(pField, ts_field_).toUTC();
        default:
            // This is for when the P-Field has no value... It can not be a valid
            // timestamp. Then it is better to just use the current time instead of giving
            // an error because there are DFHs without a timestamp where the whole
            // field is zero.
            return QDateTime::currentDateTime().toUTC();
    }
}

QDateTime
TMSourcePacketDataFieldHeader::decodeFromCDS(uint8_t pField, unsigned long *ts_field_)
{
    QDateTime ts_ = QDateTime::currentDateTime();
    unsigned char* pointer = (unsigned char*)ts_field_;

    // Skip first byte, its the pField
    pointer++;

    //Check epoch
    if (pField & 0b1000) {
        qDebug() << "Epoch bit must be zero, or we need some mission defined epoch";
        return ts_;
    }

    //Check length
    bool extendedDays = pField & 0b100;

    //Check and count days
    uint32_t days = 0;
    if (extendedDays) {
        days = (pointer[0] << 16) + (pointer[1] << 8) + pointer[2];
        pointer += 3;
    } else {
        days = (pointer[0] << 8) + pointer[1];
        pointer += 2;
    }

    //Move to POSIX epoch.
    if (days <= DAYS_CCSDS_TO_UNIX_EPOCH) {
        qDebug() << "Cannot move to Unix Epoch";
        return ts_;
    }
    days -= DAYS_CCSDS_TO_UNIX_EPOCH;
    unsigned long tv_sec = days * SECONDS_PER_DAY;
    uint32_t msDay = (pointer[0] << 24) + (pointer[1] << 16) + (pointer[2] << 8) + pointer[3];
    pointer += 4;
    tv_sec += (msDay / 1000);
    unsigned long tv_usec = 0;
    if ((pField & 0b11) == 0b01) {
            uint16_t usecs = (pointer[0] << 16) + pointer[1];
            pointer += 2;
            if (usecs > 999) {
                qDebug() << "Invalid Time Format";
                return ts_;
            }
            tv_usec += usecs;
    } else if ((pField & 0b11) == 0b10) {
            uint32_t picosecs = (pointer[0] << 24) + (pointer[1] << 16) + (pointer[2] << 8) + pointer[3];
            pointer += 4;
            if (picosecs > 999999) {
                qDebug() << "Invalid Time Format";
                return ts_;
            }
            //Not very useful.
            tv_usec += (picosecs / 1000);
    }

    unsigned long resultingTimeMs = (tv_sec * 1000) + tv_usec;
    this->setTimestampValid(true);

    ts_.setMSecsSinceEpoch(resultingTimeMs);
    return ts_;
}
