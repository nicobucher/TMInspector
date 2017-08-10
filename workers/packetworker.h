#ifndef PACKETWORKER_H
#define PACKETWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include <QHostAddress>
#include "packets/sourcepacket.h"
#include "event.h"
#include "workers/worker.h"

class PacketWorker : public Worker
{
    Q_OBJECT

public:
    PacketWorker();
    ~PacketWorker();

    QHostAddress getHost() {
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
    void setup(QThread* th_, QString *h_, quint16 *p_);

private:
    QTcpSocket* socket;
    QMutex mutex;
    QHostAddress host;
    int port;
};

#endif // PACKETWORKER_H
