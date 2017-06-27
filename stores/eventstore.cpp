#include "eventstore.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <QSettings>

extern QSettings settings;

using namespace std;

EventStore::EventStore(QObject* parent) :
    Store(parent),
    watch_list(NULL)
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

void EventStore::setModelTimestampFmt(const QString &value) {
    this->model->setMyTimestampFmt(value);
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
    qDebug() << "EventStore::Putting Event " << e_->getPacketReference();
    *this->model << e_;
    // Open the object view if the object is in the wacht list
    if (watch_list != NULL) {
        QStringList list_ = watch_list->stringList();
        QString name_ = e_->getObjectName();
        if (list_.contains(name_)) {
            emit openView(e_->getObjectName());
        }
    }
}

int EventStore::checkChildObjExists(int objId_)
{
    QStandardItem *root = this->model->invisibleRootItem();
    for (int i = 0; i < root->rowCount(); i++) {
        int test_id = root->child(i)->data(IdentifierRole).toInt();
        if (test_id == objId_) return i;
    }
    return -1;
}

void EventStore::setWatch_list(QStringListModel *value)
{
    watch_list = value;
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

QStandardItem*
EventStore::findItemInStore(QString obj_id) {
    QList<QStandardItem*> list = this->model->findItems(obj_id);
    if(list.length() == 1) {
        return list.at(0);
    } else {
        return NULL;
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
