#include "translator.h"
#include <QtCore>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMetaType>

Translator::Translator(QObject* parent, QString filename_) : QObject(parent)
{
    this->filename = filename_;
}

bool
Translator::loadHash()
{
    QFile filein(this->filename);
    if (filein.open(QIODevice::ReadOnly)) {
        QDataStream in(&filein);
        in.setVersion(QDataStream::Qt_5_5);

        in >> translation_list;
        filein.close();
        return true;
    }
    qDebug() << "Can't open file " << this->filename;
    return false;
}

bool
Translator::saveHash()
{
    QFile fileout(this->filename);
    if (fileout.open(QIODevice::WriteOnly)) {
        QDataStream out(&fileout);
        out.setVersion(QDataStream::Qt_5_5);

        out << translation_list;
        fileout.flush();
        fileout.close();
        return true;
    }
    qDebug() << "Can't open file " << this->filename;
    return false;
}

QVariant
Translator::translate(int key_) {
    QVariant return_value;
    if (translation_list.contains(key_)) {
        return_value = translation_list.value(key_);
    }
    return return_value;
}


SPIDTranslator::SPIDTranslator(QObject* parent, QString filename_) : Translator(parent, filename_)
{
    // Anything required here?
}

bool
SPIDTranslator::loadHash(QSqlDatabase* db_) {
    QString str;
    QTextStream(&str) << "SELECT PID_SPID, PID_DESCR FROM pid;";
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        translation_list.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            translation_list.insert(rec.value(0).toInt(), rec.value(1));
        }
        saveHash();
        return true;
    }
    return false;
}




PITranslator::PITranslator(QObject* parent, QString filename_) : Translator(parent, filename_)
{
    qRegisterMetaType<PI_VALUES>();
    qRegisterMetaTypeStreamOperators<PI_VALUES>();
}

bool
PITranslator::loadHash(QSqlDatabase* db_) {
    QString str;
    QTextStream(&str) << "SELECT PID_SPID, PID_PI1_VAL, PID_PI2_VAL, PID_TYPE, PID_STYPE FROM pid WHERE PID_APID=53;";
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        translation_list.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            PI_VALUES pis_;
            pis_.PI1_VAL = rec.value(1).toInt();
            pis_.PI2_VAL = rec.value(2).toInt();
            pis_.type_key = (rec.value(3).toInt() << 16) + rec.value(4).toInt();
            QVariant var_pis_;
            var_pis_.setValue(pis_);
            translation_list.insert(rec.value(0).toInt(), var_pis_);
        }
        saveHash();
        return true;
    }
    return false;
}




PICTranslator::PICTranslator(QObject* parent, QString filename_) : Translator(parent, filename_)
{
    qRegisterMetaType<PIC_VALUES>();
    qRegisterMetaTypeStreamOperators<PIC_VALUES>();
}

bool
PICTranslator::loadHash(QSqlDatabase* db_) {
    QString str;
    QTextStream(&str) << "SELECT PIC_TYPE, PIC_STYPE, PIC_PI1_OFF, PIC_PI1_WID, PIC_PI2_OFF, PIC_PI2_WID FROM pic;";
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        translation_list.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            int key_ = (rec.value(0).toInt() << 16) + rec.value(1).toInt();
            PIC_VALUES pics_;
            pics_.PI1_offset = rec.value(2).toInt();
            pics_.PI1_width = rec.value(3).toInt();
            pics_.PI2_offset = rec.value(4).toInt();
            pics_.PI2_width = rec.value(5).toInt();
            QVariant var_pics_;
            var_pics_.setValue(pics_);
            translation_list.insert(key_, var_pics_);
        }
        saveHash();
        return true;
    }
    return false;
}




EventTranslator::EventTranslator(QObject* parent, QString filename_) : Translator(parent, filename_)
{
    // Anything required here?
}

bool
EventTranslator::loadHash(QSqlDatabase* db_) {
    QString str;
    QTextStream(&str) << "SELECT Failure_Event_ID, Failure_Event_Name FROM obsw_events;";
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        translation_list.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            translation_list.insert(rec.value(0).toInt(), rec.value(1));
        }
        saveHash();
        return true;
    }
    return false;
}




ObjectTranslator::ObjectTranslator(QObject* parent, QString filename_) : Translator(parent, filename_)
{
    // Anything required here?
}

bool
ObjectTranslator::loadHash(QSqlDatabase* db_) {
    QString str;
    QTextStream(&str) << "SELECT TXP_FROM, TXP_ALTXT FROM txp WHERE TXP_NUMBR = 'YMX00005';"; // <- All Object-IDs have the calibration id 'YMX00005'
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        translation_list.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            translation_list.insert(rec.value(0).toInt(), rec.value(1));
        }
        saveHash();
        return true;
    }
    return false;
}
