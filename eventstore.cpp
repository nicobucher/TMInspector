#include "eventstore.h"
#include <QDebug>
#include "mainwindow.h"

EventStore::EventStore(QObject* parent, QSettings* set_, QHash<QString,QString>* l_objn_, QHash<QString,QString>* l_evn_) : Store(parent), settings(set_), l_object_names(l_objn_), l_event_names(l_evn_)
{
    QStringList labels;
    labels << "Object ID" << "Event ID" << "Param 1" << "Param 2" << "Timestamp";
    this->model->setHorizontalHeaderLabels(labels);
}

void EventStore::putEvent(Event* e_)
{
    QStandardItem *root = this->model->invisibleRootItem();
    QList<QStandardItem*> new_row = prepareRow(e_->getEventId(), e_->getSeverityItem(), e_->getParam1(), e_->getParam2(), e_->getTimestamp());

    int rawObjectId = e_->getObjectIdAsInt();
    int objRowFound_ = checkChildObjExists(rawObjectId);
    if (objRowFound_ < 0) {
        QString object_id = QString::number(rawObjectId);
        // If it does not exist, add a new object to the root item
        QStandardItem* new_object = new QStandardItem("0x" + e_->getObjectIdAsString());
        if (l_object_names->contains(object_id)) {
            new_object->setData("0x" + e_->getObjectIdAsString(), Qt::ToolTipRole);
            new_object->setData(l_object_names->value(object_id), Qt::DisplayRole);
            new_object->setData(rawObjectId, RawDataRole);
        } else {
            qDebug() << "Can not find " << object_id << " in Object Translation List";
            new_object->setBackground(Qt::lightGray);
            new_object->setData("Cannot be resolved", Qt::ToolTipRole);
            new_object->setData(rawObjectId, RawDataRole);
        }
        new_object->appendRow(new_row);
        root->appendRow(new_object);
    } else {
        // Just add the new event to the already existing object
        root->child(objRowFound_)->appendRow(new_row);
    }
}

int EventStore::checkChildObjExists(int objId_)
{
    QStandardItem *root = this->model->invisibleRootItem();
    for (int i = 0; i < root->rowCount(); i++) {
        int test_id = root->child(i)->data(RawDataRole).toInt();
        if (test_id == objId_) return i;
    }
    return -1;
}

QList<QStandardItem*> EventStore::prepareRow(QStandardItem* event_id, AnimatedStandardItem* severity_item, QStandardItem* param1, QStandardItem* param2, const QDateTime timestamp)
{
    QList<QStandardItem*> row;

    row << severity_item;
    if (l_event_names->contains(event_id->text())) {
        event_id->setData(event_id->text(), Qt::ToolTipRole);
        event_id->setData(l_event_names->value(event_id->text()), Qt::DisplayRole);
    } else {
        qDebug() << "Can not find " << event_id->text() << " in Event Translation List";
        event_id->setBackground(Qt::lightGray);
    }
    row << event_id;
    row << param1;
    row << param2;
    row << new QStandardItem(timestamp.toString());
    return row;
}
