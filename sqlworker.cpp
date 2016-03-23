#include "sqlworker.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlError>

SqlWorker::SqlWorker(QString dbName_, QString hostName_, int port_, QString userName_, QString passwd_)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

//    db->setConnectOptions(); // <- if connection options are required
    db.setHostName(hostName_);
    db.setPort(port_);
    db.setDatabaseName(dbName_);
    db.setUserName(userName_);
    db.setPassword(passwd_);
}

QList<SourcePacket*>
SqlWorker::fetchPackets(QDateTime b_, QDateTime e_)
{
    QList<SourcePacket*> list;
    if (db.open()) {
        QString str;
        QTextStream(&str) << "SELECT * FROM SourcePacket WHERE timestamp < " << e_.toUTC().toMSecsSinceEpoch() << " AND timestamp > " << b_.toUTC().toMSecsSinceEpoch() << " ORDER BY timestamp";
        QSqlQuery query(str);
        QSqlRecord rec = query.record();

        while (query.next()) {
            qDebug() << "Found a packet... scc=" << query.value(rec.indexOf("sequenceCount"));

            SourcePacket packet;
            QByteArray data = query.value(rec.indexOf("data")).toByteArray();

            packet.setData((unsigned char*)data.data(), data.length());
            packet.setApid(query.value(rec.indexOf("applicationProcessId")).toInt());
            packet.setSourceSequenceCount(query.value(rec.indexOf("sequenceCount")).toInt());
            packet.setSequence((Sequence)query.value(rec.indexOf("sequenceFlags")).toInt());

            TMSourcePacketDataFieldHeader header;
            header.setServiceType(query.value(rec.indexOf("serviceType")).toInt());
            header.setSubServiceType(query.value(rec.indexOf("serviceSubtype")).toInt());
            QDateTime ts_;
            ts_.setMSecsSinceEpoch(query.value(rec.indexOf("generationTimestamp")).toULongLong());
            header.setTimestamp(ts_);
            packet.setDataFieldHeader((SourcePacketDataFieldHeader*)&header);

            if (query.value(rec.indexOf("type")).toString() == "TM") {
                packet.setSourcePacketType(0);
                packet.setQuality(GOOD);
                list << &packet;
            }
        }
    } else {
        qWarning() << db.lastError().text();
//        qDebug() << QCoreApplication::libraryPaths(); // <- Lists the used library path, can be used for debugging if the qmysql plugin can not be found
    }
    return list;
}

SqlWorker::~SqlWorker() {
    // what to do here?
}
