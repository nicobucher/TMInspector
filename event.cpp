#include "event.h"
#include "translator.h"
#include <QDebug>

Event::Event()
{
   // empty constructor
    this->event_id = new QStandardItem("");
    this->object_id = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
}

Event::Event(QDateTime ts_, Severity sev_) : timestamp(ts_), severity(sev_)
{
    this->event_id = new QStandardItem("");
    this->object_id = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
    setSeverity(sev_);
}

Event::Event(QDateTime ts_, Severity sev_, unsigned char* data_) : timestamp(ts_)
{
    this->event_id = new QStandardItem("");
    this->object_id = new QStandardItem("");
    this->event_msg = new QStandardItem("");
    this->param1 = new QStandardItem("");
    this->param2 = new QStandardItem("");
    this->severity_item = new AnimatedStandardItem("");
    setSeverity(sev_);
    makeEventfromPacketData(data_);
}

QStandardItem* Event::clone() const
{
    return new Event();
}

void Event::setEventId(int evid_)
{
    this->event_id->setText(QString::number(evid_));
    QVariant event_translation = myEventTranslator.translate(event_id->text().toInt());
    if (event_translation.isValid()) {
        this->event_id->setData(this->getEventId()->text(), Qt::ToolTipRole);
        this->event_id->setData(event_translation.toString(), Qt::DisplayRole);
    } else {
        qDebug() << "Can not find " << event_id->text() << " in Event Translation List";
        this->event_id->setBackground(Qt::lightGray);
    }
}

void
Event::makeEventfromPacketData(unsigned char* pData_)
{
    unsigned int value;
    pData_ = pData_ + 12; // Skip the DFH
//    qDebug() << (int)pData_[12] << (int)pData_[13] << (int)pData_[14] << (int)pData_[15] << (int)pData_[16]<< (int)pData_[17];
    value = (pData_[0] << 8) + pData_[1];
    this->event_id->setData(QString::number(value), Qt::DisplayRole);
    value = (pData_[2] << 24) + (pData_[3] << 16) + (pData_[4] << 8) + pData_[5];
    this->object_id->setData(QString::number(value, 16), Qt::DisplayRole);
    this->object_id->setData(value, RawDataRole);
    value = (pData_[6] << 24) + (pData_[7] << 16) + (pData_[8] << 8) + pData_[9];
    this->param1->setData(QString::number(value), Qt::DisplayRole);
    value = (pData_[10] << 24) + (pData_[11] << 16) + (pData_[12] << 8) + pData_[13];
    this->param2->setData(QString::number(value), Qt::DisplayRole);
}
