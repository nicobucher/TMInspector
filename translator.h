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

    /*
     * Method which translate an int key into any QVariant
     * the returned value is not valid (check by using QVariant::isValid())
     * if no entry in the translation hash-table is found.
     */
    virtual QVariant translate(int key_ );

    virtual bool loadHash(QString filename_);
    virtual bool loadHash(QSqlDatabase* db_) = 0;       // From DB

protected:
    QHash<int, QVariant> translation_list;
};

class SPIDTranslator : public Translator
{
public:
    SPIDTranslator(QObject* parent = 0);

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class PITranslator : public Translator
{
public:
    PITranslator(QObject* parent = 0);

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class PICTranslator : public Translator
{
public:
    PICTranslator(QObject* parent = 0);

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class EventTranslator : public Translator
{
public:
    EventTranslator(QObject* parent = 0);

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

class ObjectTranslator : public Translator
{
public:
    ObjectTranslator(QObject* parent = 0);

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};

#endif // TRANSLATOR_H
