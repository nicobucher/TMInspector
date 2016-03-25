#include "sqlworker.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>

SqlWorker::SqlWorker(QObject* parent, QSettings* settings) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

//    db->setConnectOptions(); // <- if connection options are required
    db.setHostName(settings->value("db/host").toString());
    db.setPort(settings->value("db/port").toInt());
    db.setDatabaseName(settings->value("db/database").toString());
    db.setUserName(settings->value("db/username").toString());
    db.setPassword(settings->value("db/pw").toString());

    connect(this, SIGNAL(dbAccessError(QString)), parent, SLOT(displayStatusBarMessage(QString)));
}

QList<SourcePacket*>
SqlWorker::fetchPackets(QDateTime b_, QDateTime e_)
{
    QList<SourcePacket*> list;
    if (db.open()) {
        QString str;
        QTextStream(&str) << "SELECT * FROM SourcePacket WHERE generationTimestamp < " << e_.toUTC().toMSecsSinceEpoch() << " AND generationTimestamp > " << b_.toUTC().toMSecsSinceEpoch() << " ORDER BY generationTimestamp";
        QSqlQuery query(str);

        while (query.next()) {

            QSqlRecord rec = query.record();
            qDebug() << "Found a packet... scc=" << query.value(rec.indexOf("sequenceCount"));

            SourcePacket* packet = new SourcePacket();
            QByteArray data = query.value(rec.indexOf("data")).toByteArray();

            packet->setData((unsigned char*)data.data(), data.length());
            packet->setApid(query.value(rec.indexOf("applicationProcessId")).toInt());
            packet->setSourceSequenceCount(query.value(rec.indexOf("sequenceCount")).toInt());
            packet->setSequence((Sequence)query.value(rec.indexOf("sequenceFlags")).toInt());

            TMSourcePacketDataFieldHeader* header = new TMSourcePacketDataFieldHeader();
            header->setServiceType(query.value(rec.indexOf("serviceType")).toInt());
            header->setSubServiceType(query.value(rec.indexOf("serviceSubtype")).toInt());
            QDateTime ts_;
            ts_.setMSecsSinceEpoch(query.value(rec.indexOf("generationTimestamp")).toULongLong());
            header->setTimestamp(ts_);
            packet->setDataFieldHeader((SourcePacketDataFieldHeader*)header);
            packet->setDataFieldHeaderIsPresent(1);

            if (query.value(rec.indexOf("type")).toString() == "TM") {
                packet->setSourcePacketType(0);
                packet->setQuality(GOOD);
                list << packet;
            }
        }
        emit dbAccessError("DB Success, found " + QString::number(list.count()) + " Packets");
        db.close();
    } else {
        qWarning() << db.lastError().text();
        emit dbAccessError("DB Error");

//        qDebug() << QCoreApplication::libraryPaths(); // <- Lists the used library path, can be used for debugging if the qmysql plugin can not be found
    }
    return list;
}

SqlWorker::~SqlWorker() {
    // what to do here?
}
