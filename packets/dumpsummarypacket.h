#ifndef DUMPSUMMARYPACKET_H
#define DUMPSUMMARYPACKET_H

#include <QHash>
#include "sourcepacket.h"

class DumpSummaryPacket : public SourcePacket
{
    Q_OBJECT
public:
    /*
     * Copy Constructor to copy a source packet
     */
    DumpSummaryPacket(SourcePacket &packet);

    void decode();

    QHash<uint32_t, bool> getL_sequencecounts() const;

    uint32_t getOnboardStoreObject_id() const;
    uint16_t getDumpcounter() const;
    uint8_t getDumpid() const;

    bool isComplete() const;

    int getNumberOfMissingSSC();
    int getNumberOfSSC();

    QString getObject_name() const;

    qulonglong generateUniqueId();

    QList<SourcePacket *> getL_found_packets() const;
    void checkPackets();
    void add_found_packets(const QList<SourcePacket *> &value);

    int getNumberOfFoundSSC();
protected:
    QList<SourcePacket*> l_found_packets;
    QHash<uint32_t, bool> l_sequencecounts;
    uint32_t object_id;
    QString object_name;
    uint16_t dumpcounter;
    uint8_t dumpid;
};

#endif // DUMPSUMMARYPACKET_H
