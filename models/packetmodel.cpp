#include "packetmodel.h"
#include "helpers/variantptr.h"

PacketModel::PacketModel(PacketStore* parentStore_) :
    parentStore(parentStore_),
    currentId(0)
{
    qRegisterMetaType<QVector<int> >("QVector<int>");

    QStringList labels;
    labels << "APID" << "SSC" << "Type" << "Subtype" << "Byte" << "Timestamp (UTC)" << "Description";
    setHorizontalHeaderLabels(labels);
}

PacketModel& PacketModel::operator<<(SourcePacket* packet_) {
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
    setData(index(0, 1), VariantPtr<SourcePacket>::asQVariant(packet_), ObjectRole);

    // LENGTH
    setData(index(0, 4), packet_->getDataLength()+1);

    // DATAFIELD HEADER CONTENT
    if (packet_->hasDataFieldHeader()) {
        TMSourcePacketDataFieldHeader* header_ = (TMSourcePacketDataFieldHeader*)packet_->getDataFieldHeader();
        setData(index(0, 2), header_->getServiceType());
        setData(index(0, 3), header_->getSubServiceType());
        setData(index(0, 5), header_->getTimestampString(myTimestampFmt));

        if (header_->timestampValid()) {
            setData(index(0, 5), QVariant(QBrush(QColor(0, 175, 255, 255))), Qt::ForegroundRole);
        }

        if (header_->getServiceType() == 15 && header_->getSubServiceType() == 128) {
            DumpSummaryPacket* ds_packet_ = dynamic_cast<DumpSummaryPacket*>(packet_);
            if(ds_packet_ != 0) {
                if(ds_packet_->isComplete()) {
                    setData(index(0, 1), QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 2), QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 3), QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 4), QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 5), QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
                    setData(index(0, 6), QVariant(QBrush(QColor(0, 200, 0, 127))), Qt::BackgroundColorRole);
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
    }

    setData(index(0,6), packet_->getName(), Qt::DisplayRole);

    return *this;
}
