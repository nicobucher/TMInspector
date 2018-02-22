#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "packets/sourcepacket.h"
#include "definitions.h"
#include "models/packetviewfilterproxymodel.h"
#include "models/packetmodel.h"
#include "translator.h"

class PacketModel;

/*
 * The Packet Store is a specialized Store for SourcePackets
 */
class PacketStore : public Store
{
    Q_OBJECT
public:
    PacketStore(QObject *parent = 0);

    void setModelTimestampFmt(const QString &value);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel();

    SourcePacket* getPacket(qulonglong pkt_id);
    PacketViewFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {
        return this->proxy_model;
    }

    /*
     * Takes a hash-map of sequence counts and corresponding apids and checks this store
     * if the list entries are present.
     * Returns:
     * - nothing
     */
    void checkUniqueIds(QHash<qulonglong, bool> &searchIds_);

private:
    PacketModel* model;
    QHash<qulonglong, SourcePacket*> l_packets;

signals:
    void newChecksum(qint32 address, qint16 checksum);

public slots:
    void exportToFile(QString filename_);
    void putPacket(SourcePacket* p_);
};

extern PacketStore myPacketStore;
extern PacketStore mySqlPacketStore;

#endif // PACKETSTORE_H
