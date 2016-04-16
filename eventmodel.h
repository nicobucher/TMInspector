#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include "eventstore.h"

class EventStore;

/*
 * This PacketModel class is a spcialized QStandardItemModel which tracks also the current id
 * of the last inserted packet ind order to find the packet later on.
 */
class EventModel : public QStandardItemModel
{
public:
    EventModel(EventStore* parentStore_);

  /*
   * Convenience operator for putting a new sting into the model list.
   */
    EventModel& operator<<(Event* event_);

    QList<QStandardItem*> prepareRow(Event* event_);

private:
    EventStore* parentStore;
};

#endif // EVENTMODEL_H
