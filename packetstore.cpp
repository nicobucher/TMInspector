#include "packetstore.h"
#include <iostream>

PacketStore::PacketStore(QObject* parent) : Store(parent)
{   
    QStringList labels;
    labels << "SSC" << "Type" << "Subtype" << "Byte" << "Timestamp";
    this->model->setHorizontalHeaderLabels(labels);
}

void
PacketStore::putPacket(SourcePacket* p_) {
    this->model->insertRow(0);
    model->setData(model->index(0, 0), p_->getSourceSequenceCount());
    model->setData(model->index(0, 3), p_->getDataLength());
    if (p_->hasDataFieldHeader()) {
        model->setData(model->index(0, 4), p_->getDataFieldHeader()->getTimestamp());
        model->setData(model->index(0, 1), p_->getDataFieldHeader()->getServiceType());
        model->setData(model->index(0, 2), p_->getDataFieldHeader()->getSubServiceType());
    }
}
