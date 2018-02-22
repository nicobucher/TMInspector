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

    QHash<qulonglong, bool> getL_uniqueIds() const;

    uint32_t getOnboardStoreObject_id() const;
    uint16_t getDumpcounter() const;
    qulonglong getDumpid() const;

    bool isComplete() const;
    bool isChecked() const;
    void setChecked();

    int getNumberOfMissingSSC();
    int getNumberOfSSC();

    QString getObject_name() const;

    void checkPackets();

    int getNumberOfFoundSSC();
protected:
    QHash<qulonglong, bool> l_unique_ids;
    uint32_t object_id;
    QString object_name;
    uint16_t dumpcounter;
    qulonglong dumpid;
    bool checked;
};

#endif // DUMPSUMMARYPACKET_H
