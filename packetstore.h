#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "sourcepacket.h"
#include "definitions.h"
#include "packetviewfilterproxymodel.h"

/*
 * This PacketModel class is a spcialized QStandardItemModel which tracks also the current id
 * of the last inserted packet ind order to find the packet later on.
 */
class PacketModel : public QStandardItemModel
{
public:
    PacketModel(QHash<QString,QString> &names_) {
        QStringList labels;
        labels << "SSC" << "Type" << "Subtype" << "Byte" << "Timestamp" << "Description";
        setHorizontalHeaderLabels(labels);

        l_packet_names = names_;
        currentId = 0;
    }

    /*
     * Convenience operator for putting a new sting into the model list.
     */
    PacketModel& operator<<(SourcePacket* packet_) {
        currentId++;
        insertRow(0);
        setData(index(0, 0), packet_->getSourceSequenceCount());
        // This is the hidden key information to find the item in the packet list
        setData(index(0, 0), currentId, ListIndexRole);
        setData(index(0, 3), packet_->getDataLength()+1);
        if (packet_->hasDataFieldHeader()) {
            setData(index(0, 1), packet_->getDataFieldHeader()->getServiceType());
            setData(index(0, 2), packet_->getDataFieldHeader()->getSubServiceType());
            setData(index(0, 4), packet_->getDataFieldHeader()->getTimestamp());
        }
        QHash<QString, QString>::const_iterator it = l_packet_names.find(QString::number(packet_->getSpid()));
        if (it != l_packet_names.end()) {
            QString descr_ = it.value();
            setData(index(0,5), descr_, Qt::DisplayRole);
        } else {
            setData(index(0,5), "no description available", Qt::DisplayRole);
        }

        return *this;
    }

    int getCurrentId() {
        return currentId;
    }

private:
    int currentId;
    QHash<QString,QString> l_packet_names;
};

/*
 * The Packet Store is a specialized Store for SourcePackets
 */
class PacketStore : public Store
{
public:
    PacketStore(QObject *parent, QHash<QString,QString> &names_);

    bool itemInStore(QString obj_id) {
        QList<QStandardItem*> list = this->model->findItems(obj_id);
        if(list.length() > 0) {
            return true;
        } else {
            return false;
        }
    }

    int getNumberOfItems() {
        return this->model->rowCount();
    }

    void emptyStore() {
        this->model->removeRows(0, this->model->rowCount());
    }

    QStandardItemModel* getModel() {
        return this->model;
    }

    // Returns the allocated packet reference id
    int putPacket(SourcePacket* p_);
    SourcePacket* getPacket(int pkt_id);
    PacketViewFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {
        return this->proxy_model;
    }

private:
    PacketModel* model;
    QHash<int, SourcePacket*> l_packets;
    QHash<QString,QString> l_packet_names;
//    int id;

public slots:
    void exportToFile(QString filename_);
};

#endif // PACKETSTORE_H
