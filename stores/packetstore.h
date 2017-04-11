#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "packets/sourcepacket.h"
#include "definitions.h"
#include "models/packetviewfilterproxymodel.h"
#include "translator.h"
#include "packets/dumpsummarypacket.h"

/*
 * This PacketModel class is a spcialized QStandardItemModel which tracks also the current id
 * of the last inserted packet ind order to find the packet later on.
 */
class PacketModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PacketModel(QString time_fmt_) :
        myTimestampFmt(time_fmt_) {
        qRegisterMetaType<QVector<int> >("QVector<int>");

        QStringList labels;
        labels << "APID" << "SSC" << "Type" << "Subtype" << "Byte" << "Timestamp (UTC)" << "Description";
        setHorizontalHeaderLabels(labels);
        currentId = 0;
    }

    /*
     * Convenience operator for putting a new packet into the model list.
     */
    PacketModel& operator<<(SourcePacket* packet_) {
        int apid_ = packet_->getApid();
        int lastSequenceCount_ = -1;

        currentId = packet_->getId();
        insertRow(0);

        // APID
        setData(index(0, 0), packet_->getApid());

        // SSC
        int ssc = packet_->getSourceSequenceCount();
        setData(index(0, 1), ssc);
        if (lastSequenceCounts.contains(apid_)) {
            lastSequenceCount_ = lastSequenceCounts[apid_];
        }
        if (ssc != lastSequenceCount_+1) {
            setData(index(0, 1), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundRole);
        }
        lastSequenceCounts[apid_] = ssc;
        // This is the hidden key information to find the item in the packet list
        setData(index(0, 1), currentId, ListIndexRole);

        // LENGTH
        setData(index(0, 4), packet_->getDataLength()+1);

        // DATAFIELD HEADER CONTENT
        if (packet_->hasDataFieldHeader()) {
            TMSourcePacketDataFieldHeader* header_ = (TMSourcePacketDataFieldHeader*)packet_->getDataFieldHeader();
            setData(index(0, 2), header_->getServiceType());
            setData(index(0, 3), header_->getSubServiceType());
            setData(index(0, 5), header_->getTimestampString(myTimestampFmt));

            if (header_->timestampValid()) {
                setData(index(0, 5), QVariant(QBrush(QColor(0, 0, 255, 127))), Qt::ForegroundRole);
            }

            if (header_->getServiceType() == 15 && header_->getSubServiceType() == 128) {
                DumpSummaryPacket* ds_packet_ = (DumpSummaryPacket*)packet_;
                if(ds_packet_->isComplete()) {
                    setData(index(0, 1), QVariant(QBrush(Qt::green)), Qt::BackgroundColorRole);
                    setData(index(0, 2), QVariant(QBrush(Qt::green)), Qt::BackgroundColorRole);
                    setData(index(0, 3), QVariant(QBrush(Qt::green)), Qt::BackgroundColorRole);
                    setData(index(0, 4), QVariant(QBrush(Qt::green)), Qt::BackgroundColorRole);
                    setData(index(0, 5), QVariant(QBrush(Qt::green)), Qt::BackgroundColorRole);
                    setData(index(0, 6), QVariant(QBrush(Qt::green)), Qt::BackgroundColorRole);
                } else {
                    setData(index(0, 1), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 2), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 3), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 4), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 5), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 6), QVariant(QBrush(QColor(255, 0, 0, 127))), Qt::BackgroundColorRole);
                }
            }
        }

        setData(index(0,6), packet_->getName(), Qt::DisplayRole);

        return *this;
    }

    qulonglong getCurrentId() {
        return currentId;
    }

private:
    qulonglong currentId;
    QHash<int, int> lastSequenceCounts;

    QString myTimestampFmt;
};

/*
 * The Packet Store is a specialized Store for SourcePackets
 */
class PacketStore : public Store
{
    Q_OBJECT
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
     * - A hash-map of missing source sequence counts and APID combinations
     */
    QHash<uint16_t, uint16_t> checkSequenceCounts(QHash<uint16_t, uint16_t> searchForCounts);

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

#endif // PACKETSTORE_H
