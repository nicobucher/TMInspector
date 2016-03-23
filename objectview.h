#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include <QDialog>
#include "objectfilterproxymodel.h"
#include <QStandardItemModel>

namespace Ui {
class ObjectView;
}

class ObjectView : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectView(QWidget *parent = 0, QModelIndex clickedIndex = QModelIndex(), QStandardItemModel *sourceModel = 0);
    ~ObjectView();

private:
    Ui::ObjectView *ui;
    ObjectFilterProxyModel *proxyModel;
};

#endif // OBJECTVIEW_H
