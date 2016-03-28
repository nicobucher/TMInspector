#include "eventstore.h"
#include <QDebug>
#include <QSettings>
#include "filehelpers.h"

EventStore::EventStore(QObject* parent) : Store(parent)
{
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
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("192.168.197.100");
    db.setPort(3306);
    db.setDatabaseName("flpmib");
    db.setUserName("tmtcrs");
    db.setPassword("tmtcrs");

    if (db.open()) {
        populateEventHash(&db);
        populateObjectHash(&db);
        db.close();
    } else {
        qDebug() << "SQL Error";
        return;
    }

    // Save the hash tables for reuse upon application start
    if (!FileHelpers::saveHash("event_names.dat", l_event_names)) {
        qWarning() << "Can not save hash in event_names.dat";
    }
    if (!FileHelpers::saveHash("object_names.dat", l_object_names)) {
        qWarning() << "Can not save hash in object_names.dat";
    }
    emit hashUpdated();
}

void EventStore::populateEventHash(QSqlDatabase* db_)
{
    QString str;
    QTextStream(&str) << "SELECT Failure_Event_ID, Failure_Event_Name FROM obsw_events;";
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        l_event_names.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            l_event_names.insert(rec.value(0).toString(), rec.value(1).toString());
        }
    }
}

void EventStore::populateObjectHash(QSqlDatabase* db_)
{
    QString str;
    QTextStream(&str) << "SELECT TXP_FROM, TXP_ALTXT FROM txp WHERE TXP_NUMBR = 'YMX00005';"; // <- All Object-IDs have the calibration id 'YMX00005'
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        l_object_names.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            l_object_names.insert(rec.value(0).toString(), rec.value(1).toString());
        }
    }
}

void EventStore::addTranslation(QString key_, QString trans_, int list_index_)
{
    switch(list_index_) {
    case EventListIndex:
        l_event_names.insert(key_, trans_);
        break;
    case ObjectListIndex:
        l_object_names.insert(key_, trans_);
        break;
    }
    emit hashUpdated();
}

void EventStore::putEvent(Event* e_)
{
    QStandardItem *root = this->model->invisibleRootItem();
    QList<QStandardItem*> new_row = prepareRow(e_->getEventId(), e_->getSeverity(), e_->getParam1(), e_->getParam2(), e_->getTimestamp());

    int rawObjectId = e_->getObjectIdAsInt();
    int objRowFound_ = checkChildObjExists(rawObjectId);
    if (objRowFound_ < 0) {
        QString object_id = QString::number(rawObjectId);
        // If it does not exist, add a new object to the root item
        QStandardItem* new_object = new QStandardItem("0x" + e_->getObjectIdAsString());
        if (l_object_names.contains(object_id)) {
            new_object->setData("0x" + e_->getObjectIdAsString(), Qt::ToolTipRole);
            new_object->setData(l_object_names.value(object_id), Qt::DisplayRole);
            new_object->setData(rawObjectId, RawDataRole);
        } else {
            qDebug() << "Can not find " << object_id << " in Object Translation List";
            new_object->setBackground(Qt::lightGray);
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

QList<QStandardItem*> EventStore::prepareRow(QStandardItem* event_id, const Severity severity, QStandardItem* param1, QStandardItem* param2, const QDateTime timestamp)
{
    QList<QStandardItem*> row;
    AnimatedStandardItem* severity_item = new AnimatedStandardItem("");
    switch (severity) {
    case Info:
        severity_item->setAnimation(Qt::green);
        severity_item->setText("Info");
        break;
    case Low:
        severity_item->setAnimation(Qt::cyan);
        severity_item->setText("Low");
        break;
    case Medium:
        severity_item->setAnimation(Qt::yellow);
        severity_item->setText("Medium");
        break;
    case High:
        severity_item->setAnimation(Qt::red);
        severity_item->setText("Alert");
        break;
    }
    QDateTime now = QDateTime::currentDateTime();
    if (timestamp.secsTo(now) < 60*60) { // If the event was generated within the last hour
        severity_item->animate();
    }

    row << severity_item;
    if (l_event_names.contains(event_id->text())) {
        event_id->setData(event_id->text(), Qt::ToolTipRole);
        event_id->setData(l_event_names.value(event_id->text()), Qt::DisplayRole);
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
