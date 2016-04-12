#ifndef SQLWORKER_H
#define SQLWORKER_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include <QSettings>
#include <QProgressDialog>
#include "sourcepacket.h"
#include "packetstore.h"
#include "eventstore.h"

class SqlWorker : public QObject
{
    Q_OBJECT
public:
    SqlWorker(QSettings *settings, QDateTime begin_, QDateTime end_, PacketStore *st_, EventStore *evst_, QProgressDialog* prg_);
    ~SqlWorker();
    QList<SourcePacket*> fetchPackets(QDateTime b_, QDateTime e_);
private:
    QSqlDatabase db;
    QDateTime begin;
    QDateTime end;

    PacketStore* mySqlPacketStore;
    EventStore* mySqlEventStore;

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
    void cancel();
};

#endif // SQLWORKER_H
