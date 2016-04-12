#include "packetstore.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <QDebug>
using namespace std;

PacketStore::PacketStore(QObject* parent) : Store(parent)
{   
    this->model = new PacketModel();
    this->setSourceModel(this->model);
    QStringList labels;
    labels << "SSC" << "Type" << "Subtype" << "Byte" << "Timestamp";
    this->model->setHorizontalHeaderLabels(labels);
    // Initialize the hash key
    id = 0;
}

int
PacketStore::putPacket(SourcePacket* p_) {
    id++;
    this->model->insertRow(0);

//    *this->model << p_;

    model->setData(model->index(0, 0), p_->getSourceSequenceCount());
    // This is the hidden key information to find the item in the packet list
    model->setData(model->index(0, 0), id, ListIndexRole);
    model->setData(model->index(0, 3), p_->getDataLength()+1);
    if (p_->hasDataFieldHeader()) {
        model->setData(model->index(0, 4), p_->getDataFieldHeader()->getTimestamp());
        model->setData(model->index(0, 1), p_->getDataFieldHeader()->getServiceType());
        model->setData(model->index(0, 2), p_->getDataFieldHeader()->getSubServiceType());
    }
    l_packets.insert(id, p_);
    return id;
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
