#ifndef SQLWORKER_H
#define SQLWORKER_H

#include <QObject>
//#include <QSql>
#include <QSqlQueryModel>
#include <QSettings>
#include <QProgressDialog>
#include "packets/sourcepacket.h"
#include "stores/packetstore.h"
#include "stores/eventstore.h"
#include "stores/dumpstore.h"
#include "workers/worker.h"

class SqlWorker : public Worker
{
    Q_OBJECT
public:
    SqlWorker(QSettings *settings, QDateTime begin_, QDateTime end_, QProgressDialog* prg_);
    ~SqlWorker();
    QList<SourcePacket*> fetchPackets(QDateTime b_, QDateTime e_);
    QList<qulonglong> checkDumpSummaryPacket(const QStringList ids_);

private:
    QSqlDatabase db;
    QDateTime begin;
    QDateTime end;

    QProgressDialog* progress;

    int foundPackets;
    bool quit;

signals:
    void dbAccessError(QString message);
    void progressMade(int progress);
    void newText(QString);
    void newMaxProgress(int);
    void finished();

public slots:
    void doWork();
    void abortWork();
};

#endif // SQLWORKER_H
