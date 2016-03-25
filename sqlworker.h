#ifndef SQLWORKER_H
#define SQLWORKER_H

#include <QObject>
#include <QSql>
#include <QSqlQueryModel>
#include <QSettings>
#include "sourcepacket.h"

class SqlWorker : public QObject
{
    Q_OBJECT
public:
    SqlWorker(QObject *parent, QSettings *settings);
    ~SqlWorker();
    QList<SourcePacket*> fetchPackets(QDateTime b_, QDateTime e_);
private:
    QSqlDatabase db;
signals:
    void dbAccessError(QString message);
};

#endif // SQLWORKER_H
