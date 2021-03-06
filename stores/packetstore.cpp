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
    *this->model << p_;

    l_packets.insert(p_->getUnique_id(), p_);

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

void
PacketStore::checkUniqueIds(QHash<qulonglong, bool> &searchIds_) {
    QHashIterator<qulonglong, bool> it(searchIds_);
    while (it.hasNext()) {
        it.next();
        SourcePacket* nextPkt_ = getPacket(it.key());
        if (nextPkt_ != NULL) {
            if(nextPkt_->getQuality() == Quality::GOOD) {
                searchIds_[it.key()] = true;
            }
        }
    }
}

void PacketModel::setMyTimestampFmt(const QString &value)
{
    myTimestampFmt = value;
}
