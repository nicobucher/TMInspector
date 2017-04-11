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

class SqlWorker : public QObject
{
    Q_OBJECT
public:
    SqlWorker(QSettings *settings, QDateTime begin_, QDateTime end_, QProgressDialog* prg_);
    ~SqlWorker();
    QList<SourcePacket*> fetchPackets(QDateTime b_, QDateTime e_);

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
    void dumpSummaryReceived(SourcePacket*);
    void eventReceived(Event*);
    void packetReceived(SourcePacket*);

public slots:
    void doWork();
    void abortWork();
};

#endif // SQLWORKER_H
