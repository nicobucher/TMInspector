#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include "stores/eventstore.h"
#include "translator.h"

class EventStore;

/*
 * This PacketModel class is a spcialized QStandardItemModel which tracks also the current id
 * of the last inserted packet ind order to find the packet later on.
 */
class EventModel : public QStandardItemModel
{
    Q_OBJECT
public:
    EventModel(EventStore* parentStore_, QString time_fmt_);

  /*
   * Convenience operator for putting a new sting into the model list.
   */
    EventModel& operator<<(Event* event_);

    QList<QStandardItem*> prepareRow(Event* event_);

    void setEventTranslator(EventTranslator* trans_) {
        event_translator = trans_;
    }

    void setObjectTranslator(ObjectTranslator* trans_) {
        object_translator = trans_;
    }

private:
    EventStore* parentStore;
    EventTranslator* event_translator;
    ObjectTranslator* object_translator;

    QString myTimestampFmt;

public slots:
    void datachanged_debug1(QModelIndex topleft,QModelIndex bottomright);
};

#endif // EVENTMODEL_H
