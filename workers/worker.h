#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "packets/sourcepacket.h"
#include "event.h"

class Worker : public QObject
{
        Q_OBJECT

public:
    Worker();

protected:
    void processPacket(SourcePacket *packet);

public slots:
    virtual void doWork() = 0;

signals:
    void hasError(const QString& errormessage);
    void dumpSummaryReceived(SourcePacket*);
    void eventAdded(Event*);
    void eventReceived(Event*);
    void packetReceived(SourcePacket*);
};

#endif // WORKER_H
