#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include <QDialog>
#include "models/objectfilterproxymodel.h"
#include <QStandardItemModel>
#include <stores/store.h>

namespace Ui {
class ObjectView;
}

class ObjectView : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectView(QWidget *parent = 0, QModelIndex clickedIndex = QModelIndex(), QStandardItemModel *sourceModel = 0);
    ~ObjectView();

    void setObject_id(int value);
    int getObject_id() const;

private:
    Ui::ObjectView *ui;
    ObjectFilterProxyModel *proxyModel;

    int object_id;

public slots:
    void update_view(const QModelIndex& parent, int start, int end);

signals:
    void removeThisObjectView(ObjectView* p_);
};

#endif // OBJECTVIEW_H
