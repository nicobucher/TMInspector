#ifndef PACKETSTORE_H
#define PACKETSTORE_H

#include <QObject>
#include "store.h"
#include "packets/sourcepacket.h"
#include "definitions.h"
#include "packetviewfilterproxymodel.h"
#include "translator.h"

/*
 * This PacketModel class is a spcialized QStandardItemModel which tracks also the current id
 * of the last inserted packet ind order to find the packet later on.
 */
class PacketModel : public QStandardItemModel
{
public:
    PacketModel() {
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
        TMSourcePacketDataFieldHeader* dfh_ = (TMSourcePacketDataFieldHeader*)packet_->getDataFieldHeader();
        currentId = ((qulonglong)dfh_->getTimestampSeconds() << 32) + packet_->getSourceSequenceCount();
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
            setData(index(0, 5), header_->getTimestamp());

            if (header_->timestampValid()) {
                setData(index(0, 5), QVariant(QBrush(QColor(0, 0, 255, 127))), Qt::ForegroundRole);
            }
        }

        QVariant pkt_name_ = translator->translate(packet_->getSpid());
        if (pkt_name_.isValid()) {
            setData(index(0,6), pkt_name_.toString(), Qt::DisplayRole);
        } else {
            setData(index(0,6), "no description available", Qt::DisplayRole);
        }

        return *this;
    }

    qulonglong getCurrentId() {
        return currentId;
    }

    void setTranslator(SPIDTranslator* trans_) {
        translator = trans_;
    }

private:
    qulonglong currentId;
    QHash<int, int> lastSequenceCounts;
    SPIDTranslator* translator;
};

/*
 * The Packet Store is a specialized Store for SourcePackets
 */
class PacketStore : public Store
{
public:
    PacketStore(QObject *parent, SPIDTranslator* trans_);

    bool itemInStore(QString obj_id) {
        QList<QStandardItem*> list = this->model->findItems(obj_id);
        if(list.length() > 0) {
            return true;
        } else {
            return false;
        }
    }

    // TODO: this should be used to check if a TM packet with a specific
    bool recentTMPacketInStore(int search_ssc_, int startsecs_, int actuality) {
        bool found_ = false;
        QHashIterator<qulonglong, SourcePacket*> it(l_packets);
        while (it.hasNext()) {
            it.next();
            int ssc_ = (it.key() & 0xFFFFFFFF00000000) >> 32;
            int ts_ = it.key() & 0x00000000FFFFFFFF;
            if (search_ssc_ == ssc_) {
                if (ts_ > startsecs_ - actuality) {
                    found_ = true;
                }
            }
        }
        return found_;
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
    qulonglong putPacket(SourcePacket* p_);
    SourcePacket* getPacket(qulonglong pkt_id);
    PacketViewFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {
        return this->proxy_model;
    }

private:
    PacketModel* model;
    QHash<qulonglong, SourcePacket*> l_packets;

public slots:
    void exportToFile(QString filename_);
};

#endif // PACKETSTORE_H
