#ifndef PACKETWORKER_H
#define PACKETWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include "stores/packetstore.h"
#include "stores/eventstore.h"
#include "stores/dumpstore.h"

class PacketWorker : public QObject
{
    Q_OBJECT

public:
    PacketWorker();
    ~PacketWorker();

    QString getHost() {
        return this->host;
    }

    int getPort() {
        return this->port;
    }

    bool quit;
    bool isReady;

    void setStore(PacketStore *value);

    void setEvent_store(EventStore *value);

    void setDump_store(DumpStore *value);

public slots:
    void doWork();
    void networkError(QAbstractSocket::SocketError e_);
    void setup(QThread* th_, QString h_, int p_);

signals:
    void hasError(const QString& errormessage);
    void dumpSummaryReceived(DumpSummaryPacket*);
    void eventAdded(Event*);
    void eventReceived(Event*);
    void packetReceived(SourcePacket*);

private:
    QTcpSocket* socket;
    PacketStore* store;
    EventStore* event_store;
    DumpStore* dump_store;
    QMutex mutex;
    QString host;
    int port;
};

#endif // PACKETWORKER_H
