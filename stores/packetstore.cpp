#include "packetstore.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <QDebug>
#include "packets/checksumpacket.h"
using namespace std;

PacketStore::PacketStore(QObject* parent, SPIDTranslator* trans_) : Store(parent)
{   
    this->model = new PacketModel();
    this->model->setTranslator(trans_);
    this->proxy_model = new PacketViewFilterProxyModel(this);
    this->setSourceModel(this->model);
    // Initialize the hash key
//    id = 0;
}

qulonglong
PacketStore::putPacket(SourcePacket* p_) {
    *this->model << p_;

    qulonglong id_ = this->model->getCurrentId();
    l_packets.insert(id_, p_);

    if (p_->getDataFieldHeader()->getServiceType() == 6 && p_->getDataFieldHeader()->getSubServiceType() == 9) {
        ChecksumPacket new_checksum_packet(*p_);
        emit newChecksum(new_checksum_packet.getAddress(), new_checksum_packet.getChecksum());
    }

    return id_;
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

QHash<uint16_t, uint16_t>
PacketStore::checkSequenceCounts(QHash<uint16_t, uint16_t> searchForCounts) {
    QHash<uint16_t, uint16_t> missingCounts;
    QHashIterator<uint16_t, uint16_t> it(searchForCounts);
    while (it.hasNext()) {
        it.next();
        if(searchPacketInStore(it.key(), it.value()) == NULL) {
            missingCounts.insert(it.key(), it.value());
        }
    }
    return missingCounts;
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
