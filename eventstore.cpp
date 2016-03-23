#include "eventstore.h"
#include <QDebug>
#include "filehelpers.h"

EventStore::EventStore(QObject* parent) : Store(parent)
{
    this->model = new QStandardItemModel();
    QStringList labels;
    labels << "Object ID" << "Event ID" << "Param 1" << "Param 2" << "Timestamp";
    this->model->setHorizontalHeaderLabels(labels);

    this->l_event_names = FileHelpers::loadHash("event_names.dat");
    this->l_object_names = FileHelpers::loadHash("object_names.dat");
    // --> Todo: Test Code (remove)
    l_object_names.insert("Test-Event","Test-Object");
    l_event_names.insert("1","Test-Event");
    // <--
}

void EventStore::loadTranslationTable()
{
    // Get the translation data from the database

    // Save the hash tables for reuse upon application start
    if (!FileHelpers::saveHash("event_names.dat", l_event_names)) {
        qDebug() << "Can not save hash in event_names.dat";
    }
    if (!FileHelpers::saveHash("object_names.dat", l_object_names)) {
        qDebug() << "Can not save hash in object_names.dat";
    }
}

void EventStore::putEvent(Event* e_)
{
    QStandardItem *root = this->model->invisibleRootItem();
    QList<QStandardItem*> new_row = prepareRow(e_->getEventId(), e_->getSeverity(), e_->getParam1(), e_->getParam2(), e_->getTimestamp());

    int objRowFound_ = checkChildObjExists(e_->getObjectId());
    if (objRowFound_ == 0) {
        // If it does not exist, add a new object to the root item
        QStandardItem* new_object = new QStandardItem(e_->getObjectIdAsString());
        if (l_object_names.contains(e_->getObjectIdAsString())) {
            new_object->setData(e_->getObjectIdAsString(), Qt::ToolTipRole);
            new_object->setData(l_object_names.value(e_->getObjectIdAsString()), Qt::DisplayRole);
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
    QString compareString_ = QString::number(objId_);

    QStandardItem *root = this->model->invisibleRootItem();
    for (int i = 0; i < root->rowCount(); i++) {
        if (root->child(i)->text() == compareString_) return i;
    }
    return 0;
}

int EventStore::checkChildObjExists(QStandardItem* objId_)
{
    QStandardItem *root = this->model->invisibleRootItem();
    for (int i = 0; i < root->rowCount(); i++) {
        if (root->child(i)->text() == objId_->text()) return true;
    }
    return false;
}

QList<QStandardItem*> EventStore::prepareRow(QStandardItem* event_id, const Severity severity, QStandardItem* param1, QStandardItem* param2, const QDateTime timestamp)
{
    QList<QStandardItem*> row;
    QStandardItem* severity_item = new QStandardItem("");
    switch (severity) {
    case Info:
        severity_item->setBackground(Qt::green);
        severity_item->setText("Info");
        break;
    case Low:
        severity_item->setBackground(Qt::cyan);
        severity_item->setText("Low");
        break;
    case Medium:
        severity_item->setBackground(Qt::yellow);
        severity_item->setText("Medium");
        break;
    case High:
        severity_item->setBackground(Qt::red);
        severity_item->setText("Alert");
        break;
    }

    row << severity_item;
    if (l_event_names.contains(event_id->text())) {
        event_id->setData(event_id->text(), Qt::ToolTipRole);
        event_id->setData(l_event_names.value(event_id->text()), Qt::DisplayRole);
    }
    row << event_id;
    row << param1;
    row << param2;
    row << new QStandardItem(timestamp.toString());
    return row;
}
