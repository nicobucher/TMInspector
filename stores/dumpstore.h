#ifndef DUMPSTORE_H
#define DUMPSTORE_H

#include "store.h"
#include "dumpsummary.h"
#include <QDateTime>

class DumpStore : public Store
{
public:
    DumpStore(QObject *parent);

    bool itemInStore(QString obj_id);

    int getNumberOfItems();

    void emptyStore();

    QStandardItemModel* getModel() {
        return this->model;
    }

    void setSourceModel(QAbstractItemModel* src_) {
        // TODO: Not yet needed
    }

    QSortFilterProxyModel* getProxyModel() {
        // TODO: Not yet needed
        return NULL;
    }

    void putDumpSummaryPacket(DumpSummaryPacket* dps_);

    int getNumberOfDumps();
    bool containsDumpId(qulonglong id_);
    bool containsDumpId(uint32_t obj_id_, uint8_t dump_id_);
public slots:
    void exportToFile(QString filename_);

private:
    QStandardItemModel* model;

    QHash<int, DumpSummary*> l_dumps;
    qulonglong generateId(uint32_t obj_id_, uint8_t dump_id_);
};

#endif // DUMPSTORE_H
