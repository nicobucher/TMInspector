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
    PacketWorker(PacketStore* st_, EventStore *evst_, QHash<int, PI_VALUES> &l_spids_, QHash<int, PIC_VALUES> &l_types_);
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

    QHash<int,PI_VALUES> l_spids;
    QHash<int,PIC_VALUES> l_types;
};

#endif // PACKETWORKER_H
