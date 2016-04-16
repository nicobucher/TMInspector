#include "eventmodel.h"
#include <QDebug>

EventModel::EventModel(EventStore* parentStore_) : parentStore(parentStore_) {
    QStringList labels;
    labels << "Object ID" << "Event ID" << "Param 1" << "Param 2" << "Timestamp";
    setHorizontalHeaderLabels(labels);
}

EventModel&
EventModel::operator<<(Event* event_) {
    QStandardItem *root = invisibleRootItem();
    QList<QStandardItem*> new_row = prepareRow(event_);

    int rawObjectId = event_->getObjectIdAsInt();
    int objRowFound_ = parentStore->checkChildObjExists(rawObjectId);
    if (objRowFound_ < 0) {
        QString object_id = QString::number(rawObjectId);
        // If it does not exist, add a new object to the root item
        QStandardItem* new_object = new QStandardItem("0x" + event_->getObjectIdAsString());
        if (parentStore->l_object_names->contains(object_id)) {
            new_object->setData("0x" + event_->getObjectIdAsString(), Qt::ToolTipRole);
            new_object->setData(parentStore->l_object_names->value(object_id), Qt::DisplayRole);
        } else {
            qDebug() << "Can not find " << object_id << " in Object Translation List";
            new_object->setBackground(Qt::lightGray);
            new_object->setData("Cannot be resolved", Qt::ToolTipRole);
        }
        new_object->setData(rawObjectId, RawDataRole);
        new_object->setData(-1, ListIndexRole);
        new_object->appendRow(new_row);
        root->appendRow(new_object);
    } else {
        // Just add the new event to the already existing object
        root->child(objRowFound_)->appendRow(new_row);
    }

    return *this;
}

QList<QStandardItem*>
EventModel::prepareRow(Event* event_)
{
    QList<QStandardItem*> row;
    QStandardItem* event_id = event_->getEventId();
    QStandardItem* severity_item = event_->getSeverityItem();
    severity_item->setData(event_->getPacketReference(), ListIndexRole);

    row << severity_item;
    if (parentStore->l_event_names->contains(event_id->text())) {
        event_id->setData(event_->getEventId()->text(), Qt::ToolTipRole);
        event_id->setData(parentStore->l_event_names->value(event_id->text()), Qt::DisplayRole);
    } else {
        qDebug() << "Can not find " << event_id->text() << " in Event Translation List";
        event_id->setBackground(Qt::lightGray);
    }
    row << event_id;
    row << event_->getParam1();
    row << event_->getParam2();
    row << new QStandardItem(event_->getTimestamp().toString());
    return row;
}
