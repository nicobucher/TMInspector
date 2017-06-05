#ifndef DUMPSTORE_H
#define DUMPSTORE_H

#include "store.h"
#include "dumpsummary.h"
#include "models/dumpmodel.h"
#include "event.h"
#include "models/dumpviewfilterproxymodel.h"
#include <QDateTime>

class DumpModel;

class DumpStore : public Store
{
    Q_OBJECT
public:
    DumpStore(QObject *parent = 0);

    void setModelTimestampFmt(const QString &value);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel();

    DumpViewFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {        
        return this->proxy_model;
    }

    DumpSummary* getDumpSummary(qulonglong id_);

    bool containsDumpId(qulonglong id_);

    int getNumberOfDumps();

public slots:
    void exportToFile(QString filename_);
    void putDumpSummaryPacket(SourcePacket *dps_);

private:
    DumpModel* model;
    QHash<qulonglong, DumpSummary*> l_summaries;
};

extern DumpStore myDumpStore;

#endif // DUMPSTORE_H
