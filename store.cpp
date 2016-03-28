#include "store.h"
#include <QFile>
#include <QTextStream>

Store::Store(QObject* parent) : QObject(parent)
{
    this->model = new QStandardItemModel();
    this->proxy_model = new TreeViewFilterProxyModel(this);
    this->proxy_model->setSourceModel(this->model);
}

void
Store::exportToFile(QString filename_)
{
    QFile fileout(filename_);
    if (fileout.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&fileout);

        QStandardItem* parent;
        QStandardItem* child;
        for (int i=0; i < model->rowCount(); ++i) {
            parent = model->item(i, 0);
            for (int ii=0; ii < parent->rowCount(); ++ii) {
                out << parent->data(Qt::DisplayRole).toString() << '\t';
                for (int iii = 0; iii < model->columnCount(); ++iii) {
                    child = parent->child(ii, iii);
                    out << child->data(Qt::DisplayRole).toString() << '\t';
                }
                out << endl;
            }
        }

        fileout.flush();
        fileout.close();
    }
}
