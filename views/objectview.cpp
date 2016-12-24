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

    connect(proxyModel,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(update_view(QModelIndex,int,int)));
}

ObjectView::~ObjectView()
{
    emit removeThisObjectView(this);
    delete ui;
}

void
ObjectView::update_view(const QModelIndex& parent, int start, int end)
{
    // TODO do more fancy stuff here
    this->ui->treeView->scrollToBottom();
}
