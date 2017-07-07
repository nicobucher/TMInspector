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
        this->object_name->setData("0x" + getObjectIdAsHexString(), Qt::ToolTipRole);
    } else {
        setObjectName("0x" + getObjectIdAsHexString());
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
    const unsigned char *data = (const unsigned char*)pData_.constData();
    unsigned int value = 0;
    // Skip the DFH
    value = (data[12] << 8) + data[13];
    setEventId(value);
    value = (data[14] << 24) + (data[15] << 16) + (data[16] << 8) + data[17];
    setObjectId(value);
    value = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
    this->param1->setData(QString::number(value), Qt::DisplayRole);
    value = (data[22] << 24) + (data[23] << 16) + (data[24] << 8) + data[25];
    this->param2->setData(QString::number(value), Qt::DisplayRole);
}
