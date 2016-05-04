#include "packetstore.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <QDebug>
using namespace std;

PacketStore::PacketStore(QObject* parent, SPIDTranslator* trans_) : Store(parent)
{   
    this->model = new PacketModel();
    this->model.setTranslator(trans_);
    this->proxy_model = new PacketViewFilterProxyModel(this);
    this->setSourceModel(this->model);
    // Initialize the hash key
//    id = 0;
}

int
PacketStore::putPacket(SourcePacket* p_) {
    *this->model << p_;

    int id_ = this->model->getCurrentId();
    l_packets.insert(id_, p_);
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
PacketStore::getPacket(int pkt_id)
{
    return l_packets.value(pkt_id, 0);
}
