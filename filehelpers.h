#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <QtCore>

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
};

#endif // FILEHELPERS_H
