#include "eventmodel.h"
#include <QDebug>

EventModel::EventModel(EventStore* parentStore_, QString time_fmt_) :
    parentStore(parentStore_),
    myTimestampFmt(time_fmt_)
{
    QStringList labels;
    labels << "Object ID" << "Event ID" << "Param 1" << "Param 2" << "Timestamp";
    setHorizontalHeaderLabels(labels);

//    connect(this,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(datachanged_debug1(QModelIndex,QModelIndex)));
}

EventModel&
EventModel::operator<<(Event* event_) {
    QStandardItem *root = invisibleRootItem();
    QList<QStandardItem*> new_row = prepareRow(event_);

    int object_id = event_->getObjectIdAsInt();
    QVariant obj_name_ = object_translator->translate(object_id);

    int objRowFound_ = parentStore->checkChildObjExists(object_id);
    if (objRowFound_ < 0) {
        // If it does not exist, add a new object to the root item
        QStandardItem* new_object = new QStandardItem("0x" + event_->getObjectIdAsString());

        if (obj_name_.isValid()) {
            new_object->setData("0x" + event_->getObjectIdAsString(), Qt::ToolTipRole);
            new_object->setData(obj_name_.toString(), Qt::DisplayRole);
        } else {
            qDebug() << "Can not find " << object_id << " in Object Translation List";
            new_object->setBackground(Qt::lightGray);
            new_object->setData("Cannot be resolved", Qt::ToolTipRole);
        }
        new_object->setData(obj_name_, RawDataRole);
        new_object->setData(object_id, IdentifierRole);
        new_object->setData(-1, ListIndexRole);
        new_object->appendRow(new_row);
        root->appendRow(new_object);
    } else {
        // Just add the new event to the already existing object
        root->child(objRowFound_)->appendRow(new_row);
    }

    event_->setObjectName(obj_name_.toString());

    return *this;
}

QList<QStandardItem*>
EventModel::prepareRow(Event* event_)
{
    QList<QStandardItem*> row;

    row << event_->getSeverityItem();
    row << event_->getEventId();
    row << event_->getParam1();
    row << event_->getParam2();
    row << new QStandardItem(event_->getTimestamp().toString(myTimestampFmt));

    return row;
}

void
EventModel::datachanged_debug1(QModelIndex topleft,QModelIndex bottomright)
{
    qDebug() << "Data changed in row " << topleft.row() << " to " << bottomright.row();
}
