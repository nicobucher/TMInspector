#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <QtCore>
#include "sourcepacket.h"
#include "definitions.h"

class FileHelpers
{
public:
    static QHash<QString, QString> loadHash(QString filename_)
    {
        QHash<QString, QString> hash_;
        QFile filein(filename_);
        if (filein.open(QIODevice::ReadOnly)) {
            QDataStream in(&filein);
            in.setVersion(QDataStream::Qt_5_5);

            in >> hash_;
            filein.close();
        } else {
            qDebug() << "Can't open file " << filename_;
        }
        return hash_;
    }

    static QHash<int, PI_VALUES> loadHashPI(QString filename_)
    {
        QHash<QString, QString> hash_;
        QFile filein(filename_);
        if (filein.open(QIODevice::ReadOnly)) {
            QDataStream in(&filein);
            in.setVersion(QDataStream::Qt_5_5);

            in >> hash_;
            filein.close();
        } else {
            qDebug() << "Can't open file " << filename_;
        }
        QHash<int, PI_VALUES> output_;
        QHashIterator<QString, QString> it(hash_);
        while (it.hasNext()) {
            it.next();
            QStringList sPis_ = it.value().split(",");
            PI_VALUES pis_;
            pis_.PI1_VAL = sPis_.at(0).toInt();
            pis_.PI2_VAL = sPis_.at(1).toInt();
            output_.insert(it.key().toInt(), pis_);
        }

        return output_;
    }

    static QHash<int, PIC_VALUES> loadHashPIC(QString filename_)
    {
        QHash<QString, QString> hash_;
        QFile filein(filename_);
        if (filein.open(QIODevice::ReadOnly)) {
            QDataStream in(&filein);
            in.setVersion(QDataStream::Qt_5_5);

            in >> hash_;
            filein.close();
        } else {
            qDebug() << "Can't open file " << filename_;
        }
        QHash<int, PIC_VALUES> output_;
        QHashIterator<QString, QString> it(hash_);
        while (it.hasNext()) {
            it.next();
            QStringList sPics_ = it.value().split(",");
            PIC_VALUES pics_;
            pics_.PI1_offset = sPics_.at(0).toInt();
            pics_.PI1_width = sPics_.at(1).toInt();
            pics_.PI2_offset = sPics_.at(2).toInt();
            pics_.PI2_width = sPics_.at(3).toInt();
            output_.insert(it.key().toInt(), pics_);
        }

        return output_;
    }

    static bool saveHash(QString filename_, QHash<QString, QString> hash_)
    {
        QFile fileout(filename_);
        if (fileout.open(QIODevice::WriteOnly)) {
            QDataStream out(&fileout);
            out.setVersion(QDataStream::Qt_5_5);

            out << hash_;
            fileout.flush();
            fileout.close();
            return true;
        }
        return false;
    }

    static bool saveHash(QString filename_, QHash<int, PI_VALUES> input_)
    {
        QHash<QString, QString> hash_;

        QHashIterator<int, PI_VALUES> it(input_);
        while (it.hasNext()) {
            it.next();
            QString sPis_ = QString::number(it.value().PI1_VAL) + "," + QString::number(it.value().PI2_VAL);
            hash_.insert(QString::number(it.key()), sPis_);
        }

        QFile fileout(filename_);
        if (fileout.open(QIODevice::WriteOnly)) {
            QDataStream out(&fileout);
            out.setVersion(QDataStream::Qt_5_5);

            out << hash_;
            fileout.flush();
            fileout.close();
            return true;
        }
        return false;
    }

    static bool saveHash(QString filename_, QHash<int, PIC_VALUES> input_)
    {
        QHash<QString, QString> hash_;

        QHashIterator<int, PIC_VALUES> it(input_);
        while (it.hasNext()) {
            it.next();
            QString sPics_ = QString::number(it.value().PI1_offset) + "," + QString::number(it.value().PI1_width) + "," + QString::number(it.value().PI2_offset) + "," + QString::number(it.value().PI2_width);
            hash_.insert(QString::number(it.key()), sPics_);
        }

        QFile fileout(filename_);
        if (fileout.open(QIODevice::WriteOnly)) {
            QDataStream out(&fileout);
            out.setVersion(QDataStream::Qt_5_5);

            out << hash_;
            fileout.flush();
            fileout.close();
            return true;
        }
        return false;
    }
};

#endif // FILEHELPERS_H
