#include "packetstore.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <QDebug>
#include <QSettings>
#include "packets/checksumpacket.h"
#include "stores/dumpstore.h"

extern QSettings settings;

using namespace std;

PacketStore::PacketStore(QObject* parent) :
    Store(parent)
{   
    this->model = new PacketModel(this);
    this->proxy_model = new PacketViewFilterProxyModel(this);
    this->setSourceModel(this->model);
    // Initialize the hash key
//    id = 0;
}

void PacketStore::setModelTimestampFmt(const QString &value) {
    this->model->setMyTimestampFmt(value);
}

bool PacketStore::itemInStore(QString obj_id) {
    QList<QStandardItem*> list = this->model->findItems(obj_id);
    if(list.length() > 0) {
        return true;
    } else {
        return false;
    }
}

int PacketStore::getNumberOfItems() {
    return this->model->rowCount();
}

void PacketStore::emptyStore() {
    this->model->removeRows(0, this->model->rowCount());
}

QStandardItemModel* PacketStore::getModel() {
    return this->model;
}

void PacketStore::putPacket(SourcePacket* p_) {

//    DumpSummaryPacket* ds_packet = dynamic_cast<DumpSummaryPacket*>(p_);
//    if (ds_packet!=0) {
//        QHash<uint16_t, uint16_t> missingCounts = checkSequenceCounts(ds_packet->getL_sequencecounts());

//        ds_packet->setL_missing_sequencecounts(missingCounts);
//        DumpSummary* summary = myDumpStore.getDumpSummary(ds_packet->generateUniqueId());
//        summary->addMissingCounts(missingCounts);
//    }

    *this->model << p_;

    l_packets.insert(p_->getId(), p_);

    if (p_->getDataFieldHeader()->getServiceType() == 6 && p_->getDataFieldHeader()->getSubServiceType() == 10) {
        ChecksumPacket new_checksum_packet(*p_);
        emit newChecksum(new_checksum_packet.getAddress(), new_checksum_packet.getChecksum());
    }
}

void
PacketStore::exportToFile(QString filename_)
{
    ofstream fileout;
    fileout.open(filename_.toStdString().c_str(), ios::out| ios::trunc);
    if (fileout.is_open()) {
        fileout << export_file_header.toStdString();
        QDateTime now = QDateTime::currentDateTime();

        QStandardItem* print_item;
        for (int i=0; i < model->rowCount(); ++i) {
            print_item = model->item(i, 0);
            fileout << setfill(' ') << setw(5) << print_item->data(Qt::DisplayRole).toString().toStdString();
            for (int iii = 1; iii < model->columnCount(); ++iii) {
                print_item = model->item(i, iii);
                fileout << setfill(' ') << setw(30) << print_item->data(Qt::DisplayRole).toString().toStdString();
            }
            fileout << endl;
        }
        fileout << "# Generation Time: " << now.toString("ddd MMMM d yyyy").toStdString() << ", " << now.toString("hh:mm:ss").toStdString() << endl;
        fileout.flush();
        fileout.close();
    } else {
        qDebug() << "Error opening file: " << filename_;
    }
}

SourcePacket*
PacketStore::getPacket(qulonglong pkt_id)
{
    return l_packets.value(pkt_id, 0);
}

SourcePacket*
PacketStore::searchPacketInStore(uint16_t ssc_, uint16_t apid_) {
    SourcePacket* found_ = NULL;
    QHashIterator<qulonglong, SourcePacket*> it(l_packets);
    while (it.hasNext()) {
        it.next();
        if (it.value()->getSourceSequenceCount() == ssc_ && apid_ == it.value()->getApid()) {
            found_ = it.value();
        }
    }
    return found_;
}

SourcePacket*
PacketStore::searchPacketInStore(uint16_t ssc_, uint16_t apid_, QDateTime from_time_, int seconds_) {
    SourcePacket* packet = searchPacketInStore(ssc_, apid_);
    if(packet != NULL) {
        if (packet->getDataFieldHeader()->getTimestamp().secsTo(from_time_) > seconds_) {
            packet = NULL;
        }
    }
    return packet;
}

QList<SourcePacket*>
PacketStore::checkSequenceCounts(QHash<uint32_t, bool> &searchForCounts) {
    QList<SourcePacket*> foundPackets;
    QHashIterator<uint32_t, bool> it(searchForCounts);
    while (it.hasNext()) {
        it.next();
        uint16_t apid_ = (it.key() & 0xFFFF0000) >> 16;
        uint16_t ssc_ = it.key() & 0x0000FFFF;
        SourcePacket* nextPkt_ = searchPacketInStore(ssc_, apid_);
        if(nextPkt_ != NULL) {
            foundPackets.append(nextPkt_);
            searchForCounts[it.key()] = true;
        }
    }
    return foundPackets;
}

QHash<uint16_t, uint16_t>
PacketStore::checkSequenceCounts(QHash<uint16_t, uint16_t> searchForCounts, QDateTime from_time_, int seconds_) {
    QHash<uint16_t, uint16_t> missingCounts;
    QHashIterator<uint16_t, uint16_t> it(searchForCounts);
    while (it.hasNext()) {
        it.next();
        if(searchPacketInStore(it.key(), it.value(), from_time_, seconds_) == NULL) {
            missingCounts.insert(it.key(), it.value());
        }
    }
    return missingCounts;
}

void PacketModel::setMyTimestampFmt(const QString &value)
{
    myTimestampFmt = value;
}
