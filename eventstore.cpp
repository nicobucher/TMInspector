#include "eventstore.h"
#include <QDebug>
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

EventStore::EventStore(QObject* parent, QSettings* set_, QHash<QString,QString>* l_objn_, QHash<QString,QString>* l_evn_) : Store(parent), l_object_names(l_objn_), l_event_names(l_evn_), settings(set_)
{
    this->model = new EventModel(this);
    this->proxy_model = new TreeViewFilterProxyModel(this);
    this->setSourceModel(this->model);

    QTextStream(&export_file_header) << "# Log File Generated by TMInspector (Institute of Space Systems) Software" << endl;
    QTextStream(&export_file_header) << "#" << endl;
    QTextStream(&export_file_header) << "#                  Object-Name"
                                     << "                      Severity"
                                     << "                      Event-ID"
                                     << "                        Param1"
                                     << "                        Param2"
                                     << "                     Timestamp" << endl;
    QTextStream(&export_file_header) << "#" << endl;
}

void
EventStore::exportToFile(QString filename_)
{
    ofstream fileout;
    fileout.open(filename_.toStdString().c_str(), ios::out| ios::trunc);
    if (fileout.is_open()) {
        fileout << export_file_header.toStdString();
        QDateTime now = QDateTime::currentDateTime();

        QStandardItem* parent;
        QStandardItem* child;
        for (int i=0; i < model->rowCount(); ++i) {
            parent = model->item(i, 0);
            for (int ii=0; ii < parent->rowCount(); ++ii) {
                fileout << setfill(' ') << setw(30) << parent->data(Qt::DisplayRole).toString().toStdString();
                for (int iii = 0; iii < model->columnCount(); ++iii) {
                    child = parent->child(ii, iii);
                    fileout << setfill(' ') << setw(30) << child->data(Qt::DisplayRole).toString().toStdString();
                }
                fileout << endl;
            }
        }
        fileout << "# Generation Time: " << now.toString("ddd MMMM d yyyy").toStdString() << ", " << now.toString("hh:mm:ss").toStdString() << endl;
        fileout.flush();
        fileout.close();
    } else {
        qDebug() << "Error opening file: " << filename_;
    }
}

void EventStore::putEvent(Event* e_)
{
    *this->model << e_;

//    QStandardItem *root = this->model->invisibleRootItem();
//    QList<QStandardItem*> new_row = prepareRow(e_);

//    int rawObjectId = e_->getObjectIdAsInt();
//    int objRowFound_ = checkChildObjExists(rawObjectId);
//    if (objRowFound_ < 0) {
//        QString object_id = QString::number(rawObjectId);
//        // If it does not exist, add a new object to the root item
//        QStandardItem* new_object = new QStandardItem("0x" + e_->getObjectIdAsString());
//        if (l_object_names->contains(object_id)) {
//            new_object->setData("0x" + e_->getObjectIdAsString(), Qt::ToolTipRole);
//            new_object->setData(l_object_names->value(object_id), Qt::DisplayRole);
//        } else {
//            qDebug() << "Can not find " << object_id << " in Object Translation List";
//            new_object->setBackground(Qt::lightGray);
//            new_object->setData("Cannot be resolved", Qt::ToolTipRole);
//        }
//        new_object->setData(rawObjectId, RawDataRole);
//        new_object->setData(-1, ListIndexRole);
//        new_object->appendRow(new_row);
//        root->appendRow(new_object);
//    } else {
//        // Just add the new event to the already existing object
//        root->child(objRowFound_)->appendRow(new_row);
//    }
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

void EventStore::clear_hash(int list_)
{
    switch(list_) {
    case EventListIndex:
        l_event_names->clear();
        break;
    case ObjectListIndex:
        l_object_names->clear();
        break;
    }
}

bool
EventStore::itemInStore(QString obj_id) {
    QList<QStandardItem*> list = this->model->findItems(obj_id);
    if(list.length() > 0) {
        return true;
    } else {
        return false;
    }
}

int
EventStore::getNumberOfItems() {
    return this->model->rowCount();
}


void
EventStore::emptyStore() {
    this->model->removeRows(0, this->model->rowCount());
}


QStandardItemModel*
EventStore::getModel() {
    return this->model;
}

//QList<QStandardItem*> EventStore::prepareRow(Event* e_)
//{
//    QList<QStandardItem*> row;
//    QStandardItem* event_id = e_->getEventId();
//    QStandardItem* severity_item = e_->getSeverityItem();
//    severity_item->setData(e_->getPacketReference(), ListIndexRole);

//    row << severity_item;
//    if (l_event_names->contains(event_id->text())) {
//        event_id->setData(e_->getEventId()->text(), Qt::ToolTipRole);
//        event_id->setData(l_event_names->value(event_id->text()), Qt::DisplayRole);
//    } else {
//        qDebug() << "Can not find " << event_id->text() << " in Event Translation List";
//        event_id->setBackground(Qt::lightGray);
//    }
//    row << event_id;
//    row << e_->getParam1();
//    row << e_->getParam2();
//    row << new QStandardItem(e_->getTimestamp().toString());
//    return row;
//}
