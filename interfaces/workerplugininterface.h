#ifndef WORKERPLUGININTERFACE_H
#define WORKERPLUGININTERFACE_H

#include <QtPlugin>
QT_BEGIN_NAMESPACE
class SourcePacket;
QT_END_NAMESPACE

class WorkerPluginInterface
{
public:
    virtual ~WorkerPluginInterface() {}

    virtual SourcePacket* process(SourcePacket*) = 0;

//signals:
//    void eventReceived(Event*);
//    void packetReceived(SourcePacket*);
//    void dumpSummaryReceived(SourcePacket*);
};

Q_DECLARE_INTERFACE(WorkerPluginInterface, "org.TMInspector.WorkerPluginInterface")

#endif // WORKERPLUGININTERFACE_H
