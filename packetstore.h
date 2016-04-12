#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "sourcepacket.h"
#include "definitions.h"

class PacketModel : public QStandardItemModel
{
public:
  /*
   * Convenience operator for putting a new sting into the model list.
   */
  PacketModel& operator<<(SourcePacket* packet_) {

    return *this;
  }
};

class PacketStore : public Store
{
public:
    PacketStore(QObject *parent);

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

private:
    PacketModel* model;
    QHash<int, SourcePacket*> l_packets;
    int id;

public slots:
    void exportToFile(QString filename_);
};

#endif // PACKETSTORE_H
