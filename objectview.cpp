#include "objectview.h"
#include "ui_objectview.h"

ObjectView::ObjectView(QWidget *parent, QModelIndex clickedIndex, QStandardItemModel* sourceModel) :
    QDialog(parent),
    ui(new Ui::ObjectView)
{
    this->ui->setupUi(this);
    this->setWindowTitle(clickedIndex.data(Qt::DisplayRole).toString());
    this->ui->label->setText("Object-Adress: " + clickedIndex.data(Qt::ToolTipRole).toString());

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
}

ObjectView::~ObjectView()
{
    delete ui;
}
