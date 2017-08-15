#ifndef WORKERPLUGININTERFACE_H
#define WORKERPLUGININTERFACE_H

#include <packets/sourcepacket.h>
#include <event.h>

class WorkerPluginInterface
{
public:
    virtual ~WorkerPluginInterface() {}

    virtual SourcePacket* process(SourcePacket*) = 0;

signals:
    void eventReceived(Event*);
    void packetReceived(SourcePacket*);
    void dumpSummaryReceived(SourcePacket*);
};

Q_DECLARE_INTERFACE(WorkerPluginInterface, "TMInspector.WorkerPluginInterface")

#endif // WORKERPLUGININTERFACE_H
