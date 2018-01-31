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
    QHash<uint16_t, DumpSummaryPacket*> getSummaryPackets() {
        return l_summary_packets;
    }

    QDateTime getReception_time() const;

    bool isFresh();

    void addMissingCounts(QHash<uint16_t, uint16_t> counts_);
    QHash<uint16_t, uint16_t>* getMissingCounts();

    uint32_t getObject_id() const;

    QString getObject_name() const;
    void setObject_name(const QString &value);

    qulonglong getDumpId() const;
    void setDumpId(const qulonglong &value);

    QList<SourcePacket *> getL_found_packets() const;
    void add_found_packets(const QList<SourcePacket *> &value);

private:
    /*
     * A dump summary is identified by a combination of onboard object id and
     * dump id.
     */
    qulonglong dumpId;
    uint32_t object_id;
    QString object_name;
    QHash<uint16_t, DumpSummaryPacket*> l_summary_packets;
    QHash<uint16_t, uint16_t> l_missingcounts;
    QList<SourcePacket*> l_found_packets;
    QDateTime reception_time;
};

#endif // DUMPSUMMARY_H
