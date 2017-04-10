#ifndef DUMPSTORE_H
#define DUMPSTORE_H

#include "store.h"
#include "dumpsummary.h"
#include "models/dumpmodel.h"
#include <QDateTime>

class DumpModel;

class DumpStore : public Store
{
public:
    DumpStore(QObject *parent);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel();

    QSortFilterProxyModel* proxy_model;

    void setSourceModel(QAbstractItemModel* src_) {
        this->proxy_model->setSourceModel(src_);
    }

    QSortFilterProxyModel* getProxyModel() {        
        return this->proxy_model;
    }

    void putDumpSummaryPacket(DumpSummaryPacket* dps_);

    int getNumberOfDumps();
    bool containsDumpId(qulonglong id_);
    bool containsDumpId(uint32_t obj_id_, uint8_t dump_id_);
    DumpSummary* getDumpSummary(qulonglong id_);
    DumpSummary* getDumpSummary(uint8_t dump_id, uint32_t object_id);

public slots:
    void exportToFile(QString filename_);

private:
    DumpModel* model;

    QHash<qulonglong, DumpSummary*> l_dumps;
    qulonglong generateId(uint32_t obj_id_, uint8_t dump_id_);
};

#endif // DUMPSTORE_H
