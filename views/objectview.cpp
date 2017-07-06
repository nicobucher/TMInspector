#include "objectview.h"
#include "ui_objectview.h"
#include "definitions.h"

ObjectView::ObjectView(QWidget *parent, QModelIndex clickedIndex, QStandardItemModel* sourceModel) :
    QDialog(parent),
    ui(new Ui::ObjectView)
{
    this->ui->setupUi(this);

    setObject_id(clickedIndex.data(RawDataRole).toInt());

    QString windowtitle = clickedIndex.data(Qt::DisplayRole).toString();
    if (("0x" + clickedIndex.data(RawDataRole).toString()) != clickedIndex.data(Qt::DisplayRole)) {
        windowtitle.append(" (0x" + clickedIndex.data(RawDataRole).toString() + ")");
    }
    this->setWindowTitle(windowtitle);

    this->ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->treeView->setRootIsDecorated(false);
    this->ui->treeView->setUniformRowHeights(true);   // <- should increase performance

    proxyModel = new ObjectFilterProxyModel(this, clickedIndex);
    proxyModel->setSourceModel(sourceModel);
    this->ui->treeView->setModel(proxyModel);
    QModelIndex index;
    if (sourceModel->invisibleRootItem()->hasChildren()) {
        QModelIndex sourceIndex = sourceModel->invisibleRootItem()->child(0)->index();
        index = proxyModel->index(sourceIndex.row(), sourceIndex.column());
    }
    this->ui->treeView->setRootIndex(index);

    connect(proxyModel,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(update_view(QModelIndex,int,int)));
}

ObjectView::~ObjectView()
{
    emit removeThisObjectView(this);
    delete ui;
}

int ObjectView::getObject_id() const
{
    return object_id;
}

void ObjectView::setObject_id(int value)
{
    object_id = value;
}

void
ObjectView::update_view(const QModelIndex& parent, int start, int end)
{
    // TODO do more fancy stuff here
    this->ui->treeView->scrollToBottom();
}
