#ifndef SQLWORKER_H
#define SQLWORKER_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include "sourcepacket.h"

class SqlWorker
{
public:
    SqlWorker(QString dbName_, QString hostName_, int port_, QString userName_, QString passwd_);
    ~SqlWorker();
    QList<SourcePacket*> fetchPackets(QDateTime b_, QDateTime e_);
private:
    QSqlDatabase db;
};

#endif // SQLWORKER_H
