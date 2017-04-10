#ifndef DUMPMODEL_H
#define DUMPMODEL_H

#include <QStandardItemModel>
#include "stores/dumpstore.h"

class DumpStore;

class DumpModel : public QStandardItemModel
{
    Q_OBJECT
public:
    DumpModel(DumpStore* parentStore_, QString time_fmt);

    DumpModel& operator <<(DumpSummary *dump_summary_);
    DumpModel& operator <<(DumpSummaryPacket *dump_summary_packet_);

    int getNumberOfDumps();
    int checkDumpExists(uint8_t dumpId_);
    void clear();

private:
    DumpStore* parentStore;

    QString myTimestampFmt;
    void appendSummaryPacket(QStandardItem *item_, DumpSummaryPacket *dump_summary_packet_);
};

#endif // DUMPMODEL_H
