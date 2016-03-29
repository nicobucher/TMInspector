#ifndef PACKETWORKER_H
#define PACKETWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include "packetstore.h"
#include "eventstore.h"

class PacketWorker : public QObject
{
    Q_OBJECT

public:
    PacketWorker(PacketStore* st_, EventStore *evst_);
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
    void eventAdded(Event*);

private:
    QTcpSocket* socket;
    PacketStore* store;
    EventStore* event_store;
    QMutex mutex;
    QString host;
    int port;
};

#endif // PACKETWORKER_H
