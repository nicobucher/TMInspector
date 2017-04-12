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

public slots:
    void doWork();
    void networkError(QAbstractSocket::SocketError e_);
    void setup(QThread* th_, QString h_, int p_);

signals:
    void hasError(const QString& errormessage);
    void dumpSummaryReceived(SourcePacket*);
    void eventAdded(Event*);
    void eventReceived(Event*);
    void packetReceived(SourcePacket*);

private:
    QTcpSocket* socket;
    QMutex mutex;
    QString host;
    int port;
};

#endif // PACKETWORKER_H
