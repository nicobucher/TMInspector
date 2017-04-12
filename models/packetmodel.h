#ifndef PACKETMODEL_H
#define PACKETMODEL_H

#include "stores/packetstore.h"
#include "packets/dumpsummarypacket.h"

class PacketStore;

/*
 * This PacketModel class is a spcialized QStandardItemModel which tracks also the current id
 * of the last inserted packet ind order to find the packet later on.
 */
class PacketModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PacketModel(PacketStore* parentStore_);

    /*
     * Convenience operator for putting a new packet into the model list.
     */
    PacketModel& operator<<(SourcePacket* packet_);

    qulonglong getCurrentId() {
        return currentId;
    }

    void setMyTimestampFmt(const QString &value);

private:
    PacketStore* parentStore;

    qulonglong currentId;
    QHash<int, int> lastSequenceCounts;

    QString myTimestampFmt;
};


#endif // PACKETMODEL_H
