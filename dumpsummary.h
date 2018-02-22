#ifndef DUMPSUMMARY_H
#define DUMPSUMMARY_H

#include <QObject>
#include "packets/dumpsummarypacket.h"

class DumpSummary : public QObject
{
    Q_OBJECT
public:
    DumpSummary(QObject *parent, DumpSummaryPacket *init_packet);

    void putDumpSummaryPacket(DumpSummaryPacket* pkt_);
    QHash<qulonglong, DumpSummaryPacket*> getSummaryPackets();

    QDateTime getReception_time() const;

    bool isFresh();

    QHash<uint16_t, uint16_t>* getMissingPackets();

    uint32_t getObject_id() const;

    QString getObject_name() const;
    void setObject_name(const QString &value);

    qulonglong getDumpId() const;
    void setDumpId(const qulonglong &value);

    QHash<qulonglong, bool> getL_packets() const;
    void add_found_packet(SourcePacket *&value);
    void add_found_packets(const QHash<qulonglong, bool> &value);

    PacketStore *getStorePointer() const;
    void setStorePointer(PacketStore *value);

private:
    /*
     * A dump summary is identified by a combination of onboard object id and
     * dump id.
     */
    qulonglong dumpId;
    uint32_t object_id;
    QString object_name;
    /*
     * l_summary_packets
     *
     * List of each summary packet in this dump summary
     * key: the unique id of the summary packet
     * value: the pointer to the actual summary packet
     */
    QHash<qulonglong, DumpSummaryPacket*> l_summary_packets;
    /*
     * l_unique_ids
     *
     * List of unique packet ids contained in this dump summary
     * (unique ids of each dump summary packet of this dump summary),
     * key: The actual unique id
     * value: "true" if the packet has been found in the corresponding store
     */
    QHash<qulonglong, bool> l_unique_ids;
    QDateTime reception_time;
    PacketStore* storePointer;
};

#endif // DUMPSUMMARY_H
