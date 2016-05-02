#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include "definitions.h"

class Translator : public QObject
{
    Q_OBJECT
public:
    Translator(QObject* parent = 0);

    void loadHash(QHash<int, QVariant> hash_) {    // Copy from Reference
        translation_list = hash_;
    }

    void addEntry(const int key_, const QVariant value_) {
        translation_list.insert(key_, value_);
    }

    void deleteEntry(const int key_) {
        translation_list.remove(key_);
    }

    virtual bool loadHash(const QString filename_); // From File
    virtual bool saveHash(const QString filename_);    // To File

    virtual QString translate( QString st_ ) = 0;
    virtual bool loadHash(QSqlDatabase* db_) = 0;    // From DB


protected:
    QHash<int, QVariant> translation_list;
};

class SPIDTranslator : public Translator
{
public:
    SPIDTranslator(QObject* parent = 0);

    QString translate( QString st_ );
    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class PITranslator : public Translator
{
public:
    PITranslator(QObject* parent = 0);

    QString translate( QString st_ );
    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class PICTranslator : public Translator
{
public:
    PICTranslator(QObject* parent = 0);

    QString translate( QString st_ );
    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class EventTranslator : public Translator
{
public:
    EventTranslator(QObject* parent = 0);

    QString translate( QString st_ );
    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class ObjectTranslator : public Translator
{
public:
    ObjectTranslator(QObject* parent = 0);

    QString translate( QString st_ );
    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

#endif // TRANSLATOR_H
