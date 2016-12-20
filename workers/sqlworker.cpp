#include "sqlworker.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>

SqlWorker::SqlWorker(QSettings* settings, QDateTime begin_, QDateTime end_, PacketStore* st_, EventStore *evst_, QProgressDialog* prg_, QHash<int, QVariant> *l_pis_, QHash<int, QVariant> *l_pics_)
    : begin(begin_),
      end(end_),
      mySqlPacketStore(st_),
      mySqlEventStore(evst_),
      progress(prg_),
      l_pis(l_pis_),
      l_pics(l_pics_)
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
            qApp->processEvents();
            if (this->quit) {
                break;
            }

            emit progressMade(i++);

            QSqlRecord rec = query.record();
            qDebug() << "Found a packet... scc=" << query.value(rec.indexOf("sequenceCount"));

            SourcePacket* packet = new SourcePacket();
            QByteArray data = query.value(rec.indexOf("data")).toByteArray();

            packet->setVersion(0);
            packet->setDataField((unsigned char*)data.data()+packet->HEADER_SIZE, data.length()-packet->HEADER_SIZE);
            packet->setApid(query.value(rec.indexOf("applicationProcessId")).toInt());
            packet->setSourceSequenceCount(query.value(rec.indexOf("sequenceCount")).toInt());
            packet->setSequence((Sequence)query.value(rec.indexOf("sequenceFlags")).toInt());

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
            packet->setDataFieldHeader(header);

            if (query.value(rec.indexOf("type")).toString() == "TM") {
                packet->setSourcePacketType(0);
                packet->setQuality(GOOD);
                packet->makePI_VALUES(l_pics);
                packet->makeSPID(l_pis);
                list << packet;
            }
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
        emit newText("Adding Packets...");
        unsigned char* complete_packet_data = (unsigned char*) malloc(SourcePacket::MAX_PACKET_SIZE); // Maximum TM packet size
        for (int i = 0; i < retrievedPackets.size(); ++i) {
            qApp->processEvents();
            if (this->quit) {
                break;
            }
            emit progressMade(foundPackets+i);

            SourcePacket* packet = retrievedPackets.at(i);
            int ref_ = mySqlPacketStore->putPacket(packet);

            if (packet->hasDataFieldHeader()) {
                if (packet->getDataFieldHeader()->getServiceType() == 5) {
                    Event* event = new Event(packet->getDataFieldHeader()->getTimestamp(), (Severity)packet->getDataFieldHeader()->getSubServiceType());
                    int data_length = packet->getDataLength();
                    if ( data_length > 0 && data_length < SourcePacket::MAX_PACKET_SIZE ) {
                        memcpy(complete_packet_data+12, packet->getData(), packet->getDataLength());
                        event->makeEventfromPacketData(complete_packet_data);
                    }
                    event->setPacketReference(ref_);
                    // Put the event into the event store
                    mySqlEventStore->putEvent(event);
                }
            }
        }
        free(complete_packet_data);
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
