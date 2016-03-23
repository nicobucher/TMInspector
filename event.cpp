#include "event.h"

Event::Event()
{
   // empty constructor
    this->event_id = new QStandardItem("");
    this->object_id = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
}

Event::Event(QDateTime ts_, Severity sev_) : timestamp(ts_), severity(sev_)
{
    this->event_id = new QStandardItem("");
    this->object_id = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->param1 = new QStandardItem("");
}

Event::Event(QDateTime ts_, Severity sev_, unsigned char* data_) : timestamp(ts_), severity(sev_)
{
    this->event_id = new QStandardItem("");
    this->object_id = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
    makeEventfromPacketData(data_);
}

QStandardItem* Event::clone() const
{
    return new Event();
}

void
Event::makeEventfromPacketData(unsigned char* pData_)
{
    int value;
    value = (pData_[12] << 8) + pData_[13];
    this->event_id->setData(QString::number(value), Qt::DisplayRole);
    value = (pData_[14] << 24) + (pData_[15] << 16) + (pData_[16] << 8) + pData_[17];
    this->object_id->setData(QString::number(value, 16), Qt::DisplayRole);
    value = (pData_[18] << 24) + (pData_[19] << 16) + (pData_[20] << 8) + pData_[21];
    this->param1->setData(QString::number(value), Qt::DisplayRole);
    value = (pData_[22] << 24) + (pData_[23] << 16) + (pData_[24] << 8) + pData_[25];
    this->param2->setData(QString::number(value), Qt::DisplayRole);
}
