#include "sqlworker.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>
#include "stores/packetstore.h"

SqlWorker::SqlWorker(QSettings* settings, QDateTime begin_, QDateTime end_, QProgressDialog* prg_) :
    begin(begin_),
    end(end_),
    progress(prg_)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3"); // set timeout for database connection
    db.setHostName(settings->value("db/host").toString());
    db.setPort(settings->value("db/port").toInt());
    db.setDatabaseName(settings->value("db/database").toString());
    db.setUserName(settings->value("db/username").toString());
    db.setPassword(settings->value("db/pw").toString());

    this->foundPackets = 0;
    this->quit = false;
}

QList<SourcePacket*>
SqlWorker::fetchPackets(QDateTime b_, QDateTime e_)
{
    QList<SourcePacket*> list;
    emit newText("Connecting to " + db.databaseName() + "...");
    if (db.open()) {
        emit newText("Connected to " + db.databaseName() + "...");
        QString str;
        QTextStream(&str) << "SELECT * FROM SourcePacket WHERE generationTimestamp < " << e_.toUTC().toMSecsSinceEpoch() << " AND generationTimestamp > " << b_.toUTC().toMSecsSinceEpoch() << " ORDER BY generationTimestamp";
        QSqlQuery query(str);

        this->foundPackets = query.size();

        if (query.size() > 0) {
            emit newMaxProgress(2*foundPackets);
        }

        int i = 0;
        while (query.next()) {
            if (this->quit) {
                break;
            }

            i++;
            if (i % 1000 == 0) {
                emit progressMade(i);
            }

            QSqlRecord rec = query.record();
            qDebug() << "Found a packet... scc=" << query.value(rec.indexOf("sequenceCount")).toInt();

            SourcePacket* packet = new SourcePacket();
            QByteArray data = query.value(rec.indexOf("data")).toByteArray();

            packet->setVersion(0);
            packet->setDataField((unsigned char*)data.data()+packet->HEADER_SIZE, data.length()-packet->HEADER_SIZE);
            packet->setApid(query.value(rec.indexOf("applicationProcessId")).toInt());
            packet->setSourceSequenceCount(query.value(rec.indexOf("sequenceCount")).toInt());
            packet->setSequence((Sequence)query.value(rec.indexOf("sequenceFlags")).toInt());
            packet->setMibVersion(query.value(rec.indexOf("mibVersion")).toString());

            TMSourcePacketDataFieldHeader* header = new TMSourcePacketDataFieldHeader();
            header->setVersion(1);
            header->setServiceType(query.value(rec.indexOf("serviceType")).toInt());
            header->setSubServiceType(query.value(rec.indexOf("serviceSubtype")).toInt());
            int key_ = (header->getServiceType() << 16) + header->getSubServiceType();
            header->setTypeKey(key_);
            QDateTime ts_;
            ts_.setMSecsSinceEpoch(query.value(rec.indexOf("generationTimestamp")).toULongLong());
            header->setTimestamp(ts_);
            header->setTimestampValid(true);
            header->setCoreId(query.value(rec.indexOf("core")).toInt());
            header->setBootCount(query.value(rec.indexOf("obswBootcount")).toInt());
            packet->setDataFieldHeader(header);

            packet->setSourcePacketType(0);
            packet->setQuality(GOOD);
            packet->makePI_VALUES();
            packet->makeSPID();

            QVariant pkt_name_ = mySPIDTranslator.translate(packet->getSpid());
            if (pkt_name_.isValid()) {
                packet->setName(pkt_name_.toString());
            } else {
                packet->setName("no description available");
            }

            list << packet;
        }
        emit dbAccessError("DB Success, found " + QString::number(list.count()) + " Packets");
        db.close();
    } else {
        qWarning() << db.lastError().text();
        emit dbAccessError(db.lastError().text());

//        qDebug() << QCoreApplication::libraryPaths(); // <- Lists the used library path, can be used for debugging if the qmysql plugin can not be found
    }
    return list;
}

void
SqlWorker::doWork() {
    QList<SourcePacket*> retrievedPackets;
    retrievedPackets = fetchPackets(begin, end);
    if (retrievedPackets.size() > 0) {
        for (int i = 0; i < retrievedPackets.size(); ++i) {
            qDebug() << "Adding packet " << i;
            // This is for the progress bar to not get stuck
            if (i % 1000 == 0) {
                emit progressMade(foundPackets+i);
                emit newText("Adding Packets... (" + QString::number(i) + "/" + QString::number(foundPackets) + ")");
            }
            if (this->quit) {
                break;
            }

            SourcePacket* packet = retrievedPackets.at(i);
            packet->setStorePointer(&mySqlPacketStore);

            processPacket(packet);
        }
    }
    emit finished();
}

void
SqlWorker::abortWork() {
    this->quit = true;
}

SqlWorker::~SqlWorker() {
    // what to do here?
}
