#include "worker.h"
#include "packets/dumpsummarypacket.h"

Worker::Worker()
{

}

void Worker::processPacket(SourcePacket* packet)
{
    if (packet->hasDataFieldHeader()) {
        emit packetReceived(packet);
        switch (packet->getDataFieldHeader()->getServiceType()) {
        case 5:
            {
                Event* event = new Event(packet);
                emit eventReceived(event);
                emit eventAdded(event);
            }
            break;
        case 15:
            {
                if (packet->getDataFieldHeader()->getSubServiceType() == 128) {
                    DumpSummaryPacket* ds_packet = new DumpSummaryPacket(*packet);
                    emit dumpSummaryReceived(ds_packet);
                }
            }
            break;
        }
    }
}
