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
     * This searches the packets in the store for a given source sequence count and APID combination.
     * Takes:
     * - (uint16_t) the source sequence count of the packet to search for
     * - (uint16_t) the APID of the searched packet
     * Returns:
     * - SourcePacket* pointer to the found packet
     */
    SourcePacket* searchPacketInStore(uint16_t ssc_, uint16_t apid_);

    /*
     * This searches the packets in the store for a given source sequence count and APID combination.
     * This only returns packet which have a timestammp that is 'seconds_' newer than 'from_time_'
     * Takes:
     * - (uint16_t) the source sequence count of the packet to search for
     * - (uint16_t) the APID of the searched packet
     * - (QDateTime) reference time
     * - (int) Only packets which are 'seconds_' older than 'from_time_' are returned
     * Returns:
     * - SourcePacket* pointer to the found packet
     */
    SourcePacket* searchPacketInStore(uint16_t ssc_, uint16_t apid_, QDateTime from_time_, int seconds_);

    /*
     * Takes a hash-map of sequence counts and corresponding apids and checks this store
     * if the list entries are present.
     * Returns:
     * - A list of pointers to the corresponding source packets in the packet store
     */
    QList<SourcePacket *> checkSequenceCounts(QHash<uint32_t, bool> &searchForCounts);

    /*
     * Takes a hash-map of sequence counts and corresponding apids and checks this store
     * if the list entries are present. This version only returns a hash-map of the sequence counts
     * and APID combinations for packets which are not older than 'seconds_' before 'from_time_'.
     * Returns:
     * - A hash-map of missing source sequence counts and APID combinations in time-range
     */
    QHash<uint16_t, uint16_t> checkSequenceCounts(QHash<uint16_t, uint16_t> searchForCounts, QDateTime from_time_, int seconds_);
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
