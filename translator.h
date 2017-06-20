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
    Translator(QObject* parent = 0, QString filename_ = "default.dat");

    void loadHash(QHash<int, QVariant> hash_) {    // Copy from Reference
        translation_list = hash_;
    }

    void addEntry(const int key_, const QVariant value_) {
        translation_list.insert(key_, value_);
        saveHash();
    }

    void deleteEntry(const int key_) {
        translation_list.remove(key_);
    }

    void clear() {
        translation_list.clear();
    }

    /*
     * Method which translate an int key into any QVariant
     * the returned value is not valid (check by using QVariant::isValid())
     * if no entry in the translation hash-table is found.
     */
    virtual QVariant translate(int key_ );

    /*
     * loadHash() and saveHash() load and save the current hash table to their designated .dat files.
     */
    virtual bool loadHash();
    virtual bool saveHash();
    /*
     * While loadHash(DB) with a database as an argument fetches the tables fresh from the configured database
     * If a hash table is loaded from database, the fresh table is also saved in the .dat file (loadHash(db) calls
     * saveHash() internally).
     */
    virtual bool loadHash(QSqlDatabase* db_) = 0;

    virtual QHash<int, QVariant>* getList() {
        return &translation_list;
    }

protected:
    QString filename;
    QHash<int, QVariant> translation_list;
};



class SPIDTranslator : public Translator
{
public:
    SPIDTranslator(QObject* parent = 0, QString filename_ = "table_spids.dat");

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};




class PITranslator : public Translator
{
public:
    PITranslator(QObject* parent = 0, QString filename_ = "table_pi.dat");

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};




class PICTranslator : public Translator
{
public:
    PICTranslator(QObject* parent = 0, QString filename_ = "table_pic.dat");

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};




class EventTranslator : public Translator
{
public:
    EventTranslator(QObject* parent = 0, QString filename_ = "table_event.dat");

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};




class ObjectTranslator : public Translator
{
public:
    ObjectTranslator(QObject* parent = 0, QString filename_ = "table_objects.dat");

    bool loadHash(QSqlDatabase* db_);    // From DB
private:

};


extern SPIDTranslator mySPIDTranslator;
extern PITranslator myPITranslator;
extern PICTranslator myPICTranslator;
extern EventTranslator myEventTranslator;
extern ObjectTranslator myObjectTranslator;

#endif // TRANSLATOR_H
