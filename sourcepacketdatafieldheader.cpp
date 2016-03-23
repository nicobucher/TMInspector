#include "sourcepacketdatafieldheader.h"

SourcePacketDataFieldHeader::SourcePacketDataFieldHeader()
{
    //
}

TMSourcePacketDataFieldHeader::TMSourcePacketDataFieldHeader() : version(1)
{
    this->length = 12;
}

SourcePacketDataFieldHeader*
TMSourcePacketDataFieldHeader::makeDataFieldHeaderFromData(unsigned char *pData_)
{
    this->setVersion((*pData_ & 0x70) >> 4);
    this->setServiceType(*(pData_+1) & 0xff);
    this->setSubServiceType(*(pData_+2) & 0xff);
    this->setPacketSubCounter(*(pData_+3) & 0xff);
    unsigned long ts_raw_ = (unsigned long)*(pData_+4);
    QDateTime ts_;
    ts_.setMSecsSinceEpoch(ts_raw_);
    this->setTimestamp(ts_);

    // Todo: Remove this once timestamps are implemented -->
    QDateTime now = QDateTime::currentDateTime();
    this->setTimestamp(now);
    // <--

    return this;
}
