#ifndef PACKETWORKER_H
#define PACKETWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include "packets/sourcepacket.h"
#include "event.h"
#include "workers/worker.h"

class PacketWorker : public Worker
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

private:
    QTcpSocket* socket;
    QMutex mutex;
    QString host;
    int port;
};

#endif // PACKETWORKER_H
