#include "event.h"
#include "translator.h"

Event::Event()
{
   // empty constructor
    this->event_id = new QStandardItem("");
    this->object_name = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
}

Event::Event(QDateTime ts_, Severity sev_) : timestamp(ts_), severity(sev_)
{
    this->event_id = new QStandardItem("");
    this->object_name = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
    setSeverity(sev_);
}

Event::Event(QDateTime ts_, Severity sev_, QByteArray data_) : timestamp(ts_)
{
    this->event_id = new QStandardItem("");
    this->object_name = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
    setSeverity(sev_);
    makeEventfromPacketData(data_);
}

Event::Event(SourcePacket* packet)
{
    this->timestamp = packet->getDataFieldHeader()->getTimestamp();
    this->event_id = new QStandardItem("");
    this->object_name = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
    setSeverity((Severity)packet->getDataFieldHeader()->getSubServiceType());
    if ( packet->getDataLength() > 0 && packet->getDataLength() < SourcePacket::MAX_PACKET_SIZE ) {
        makeEventfromPacketData(packet->getData());
    }
    setPacketReference(packet->getId());
}

QStandardItem* Event::clone() const
{
    return new Event();
}

void Event::setObjectId(int id_)
{
    this->object_id = id_;
    QVariant obj_name_ = myObjectTranslator.translate(object_id);
    if (obj_name_.isValid()) {
        setObjectName(obj_name_.toString());
        this->object_name->setData("0x" + getObjectIdAsString(), Qt::ToolTipRole);
    } else {
        setObjectName("0x" + QString::number(this->object_id, 16));
        this->object_name->setData("Cannot be resolved", Qt::ToolTipRole);
        this->object_name->setBackground(Qt::lightGray);
    }
}

void Event::setEventId(int evid_)
{
    this->event_id->setText(QString::number(evid_));
    QVariant event_translation = myEventTranslator.translate(event_id->text().toInt());
    if (event_translation.isValid()) {
        this->event_id->setData(this->getEventId()->text(), Qt::ToolTipRole);
        this->event_id->setData(event_translation.toString(), Qt::DisplayRole);
    } else {
        this->event_id->setBackground(Qt::lightGray);
    }
}

void
Event::makeEventfromPacketData(QByteArray pData_)
{
    unsigned int value;
    // Skip the DFH
    value = (pData_[12] << 8) + pData_[13];
    setEventId(value);
    value = (pData_[14] << 24) + (pData_[15] << 16) + (pData_[16] << 8) + pData_[17];
    setObjectId(value);
    value = (pData_[18] << 24) + (pData_[19] << 16) + (pData_[20] << 8) + pData_[21];
    this->param1->setData(QString::number(value), Qt::DisplayRole);
    value = (pData_[22] << 24) + (pData_[23] << 16) + (pData_[24] << 8) + pData_[25];
    this->param2->setData(QString::number(value), Qt::DisplayRole);
}
