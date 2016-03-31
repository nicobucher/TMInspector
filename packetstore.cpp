#include "packetstore.h"
#include <iostream>

PacketStore::PacketStore(QObject* parent) : Store(parent)
{   
    QStringList labels;
    labels << "SSC" << "Type" << "Subtype" << "Byte" << "Timestamp";
    this->model->setHorizontalHeaderLabels(labels);
    // Initialize the hash key
    id = 0;
}

void
PacketStore::putPacket(SourcePacket* p_) {
    id++;
    this->model->insertRow(0);
    model->setData(model->index(0, 0), p_->getSourceSequenceCount());
    // This is the hidden key information to find the item in the packet list
    model->setData(model->index(0, 0), id, PacketListIndex);
    model->setData(model->index(0, 3), p_->getDataLength());
    if (p_->hasDataFieldHeader()) {
        model->setData(model->index(0, 4), p_->getDataFieldHeader()->getTimestamp());
        model->setData(model->index(0, 1), p_->getDataFieldHeader()->getServiceType());
        model->setData(model->index(0, 2), p_->getDataFieldHeader()->getSubServiceType());
    }
    l_packets.insert(id, p_);
}

SourcePacket*
PacketStore::getPacket(int pkt_id)
{
    return l_packets.value(pkt_id, 0);
}
