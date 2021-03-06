#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QStandardItem>
#include <QDateTime>
#include "animatedstandarditem.h"
#include "definitions.h"
#include "packets/sourcepacket.h"

enum Severity {
    Info = 1,
    Low,
    Medium,
    High
};

class Event : public QObject, public QStandardItem
{
    Q_OBJECT
public:
    Event();
    Event(QDateTime ts_, Severity sev_);
    Event(QDateTime ts_, Severity sev_, QByteArray data_);
    Event(SourcePacket *packet);
    virtual QStandardItem* clone() const;

private:
    QStandardItem* event_id;
    int object_id;
    QStandardItem* object_name;
    QDateTime timestamp;
    QStandardItem* event_msg;
    QStandardItem* param1;
    QStandardItem* param2;
    Severity severity;
    AnimatedStandardItem* severity_item;

    qulonglong packet_reference;

public:
    void setTimestamp(QDateTime ts_) {
        this->timestamp = ts_;
    }

    QDateTime getTimestamp() {
        return this->timestamp;
    }

    void setObjectId(int id_);

    QStandardItem* getObjectNameItem() {
        return this->object_name;
    }

    QString getObjectIdAsHexString() {
        return QString::number(this->object_id, 16);
    }

    QString getObjectIdAsDecString() {
        return QString::number(this->object_id, 10);
    }

    int getObjectIdAsInt() {
        return this->object_id;
    }

    QString getObjectName() {
        return this->object_name->data(Qt::DisplayRole).toString();
    }

    void setObjectName(QString name_) {
        this->object_name->setData(name_, Qt::DisplayRole);
    }

    void setEventId(int evid_);

    QStandardItem* getEventId() {
        return this->event_id;
    }

    void setMessage(QString evmsg_) {
        this->event_msg->setText(evmsg_);
    }

    QString getMessage() {
        return this->event_msg->text();
    }

    void setParams(int param1_, int param2_) {
        this->param1->setText(QString::number(param1_));
        this->param2->setText(QString::number(param2_));
    }

    QStandardItem* getParam1() {
        return this->param1;
    }

    QStandardItem* getParam2() {
        return this->param2;
    }

    Severity getSeverity() {
        return severity;
    }

    void setSeverity(Severity sev_) {
        this->severity = sev_;
        switch (this->severity) {
        case Info:
            this->severity_item->setAnimation(Qt::darkGreen);
            this->severity_item->setText("Info");
            break;
        case Low:
            this->severity_item->setAnimation(Qt::darkCyan);
            this->severity_item->setText("Low");
            break;
        case Medium:
            this->severity_item->setAnimation(Qt::darkYellow);
            this->severity_item->setText("Medium");
            break;
        case High:
            this->severity_item->setAnimation(Qt::darkRed);
            this->severity_item->setText("Alert");
            break;
        }
        this->severity_item->setData(getPacketReference(), ListIndexRole);
    }

    AnimatedStandardItem* getSeverityItem() {
        return this->severity_item;
    }

    void setPacketReference(qulonglong id_) {
        this->packet_reference = id_;
    }

    qulonglong getPacketReference() {
        return this->packet_reference;
    }

    void makeEventfromPacketData(QByteArray data);
};

#endif // EVENT_H
