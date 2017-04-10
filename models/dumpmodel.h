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

    DumpModel& operator <<(DumpSummary* sum_);

private:
    DumpStore* parentStore;

    QString myTimestampFmt;

};

#endif // DUMPMODEL_H
