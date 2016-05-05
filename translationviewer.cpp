#include "translationviewer.h"
#include "ui_translationviewer.h"
#include "addtranslationdialog.h"
#include <QMenu>

TranslationViewer::TranslationViewer(QWidget *parent,
                                     ObjectTranslator* obj_trans_,
                                     EventTranslator* event_trans_,
                                     SPIDTranslator* spid_trans_,
                                     PITranslator* pi_trans_,
                                     PICTranslator* pic_trans_) :
    QDialog(parent),
    ui(new Ui::TranslationViewer),
    myObjectTranslator(obj_trans_),
    myEventTranslator(event_trans_),
    mySPIDTranslator(spid_trans_),
    myPITranslator(pi_trans_),
    myPICTranslator(pic_trans_)
{
    ui->setupUi(this);

    pMainWindow = (MainWindow*)parent;

    connect(ui->pushButton_2, SIGNAL(clicked(bool)), pMainWindow, SLOT(loadTranslationTable()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addTranslation()));
    connect(ui->pushButton_clr, SIGNAL(clicked(bool)), this, SLOT(clearTranslationTable()));
    connect(pMainWindow, SIGNAL(hashUpdated()), this, SLOT(reload()));

    ui->comboBox->insertItem(EventListIndex, "Event-ID Translation List");
    ui->comboBox->insertItem(ObjectListIndex, "Object-ID Translation List");
    ui->comboBox->insertItem(SPIDListIndex, "SPID Translation List");
    ui->comboBox->insertItem(PI_VALUESListIndex, "PI_VALUES Translation List");
    ui->comboBox->insertItem(PIC_VALUESListIndex, "Packet Types Structure List");

    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(comboBoxSelected(int)));

    eventListModel = new QStandardItemModel(this);
    eventListModelProxy = new QSortFilterProxyModel(this);
    eventListModelProxy->setSourceModel(eventListModel);
    objectListModel = new QStandardItemModel(this);
    objectListModelProxy = new QSortFilterProxyModel(this);
    objectListModelProxy->setSourceModel(objectListModel);
    SPIDListModel = new QStandardItemModel(this);
    SPIDListModelProxy = new QSortFilterProxyModel(this);
    SPIDListModelProxy->setSourceModel(SPIDListModel);
    PI_VALUESListModel = new QStandardItemModel(this);
    PI_VALUESListModelProxy = new QSortFilterProxyModel(this);
    PI_VALUESListModelProxy->setSourceModel(PI_VALUESListModel);
    PIC_VALUESListModel = new QStandardItemModel(this);
    PIC_VALUESListModelProxy = new QSortFilterProxyModel(this);
    PIC_VALUESListModelProxy->setSourceModel(PIC_VALUESListModel);

    QStringList list;
    list << "Key" << "Value";
    eventListModel->setHorizontalHeaderLabels(list);
    objectListModel->setHorizontalHeaderLabels(list);
    SPIDListModel->setHorizontalHeaderLabels(list);
    PI_VALUESListModel->setHorizontalHeaderLabels(list);
    PIC_VALUESListModel->setHorizontalHeaderLabels(list);

    selectedProxy = eventListModelProxy;
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView->setModel(selectedProxy);

    reload();
    updateInfoText();

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(table_item_right_click(QPoint)));
}

TranslationViewer::~TranslationViewer()
{
    delete ui;
}

void
TranslationViewer::updateList(QHash<int, QVariant>* hash_, QStandardItemModel* model_)
{
    model_->removeRows(0, model_->rowCount());
    QHashIterator<int, QVariant> it(*hash_);
    while (it.hasNext()) {
        it.next();
        QList<QStandardItem*> row;
        QStandardItem* key = new QStandardItem();
        key->setData("0x" + QString::number(it.key(),16), Qt::DisplayRole);
        key->setData(it.key(), Qt::ToolTipRole);
        QStandardItem* value = new QStandardItem(it.value().toString());
        row << key << value;
        model_->insertRow(0, row);
    }
    model_->sort(0, Qt::AscendingOrder);
    updateInfoText();
}

void
TranslationViewer::updatePIList(QHash<int, QVariant> *hash_, QStandardItemModel* model_)
{
    model_->removeRows(0, model_->rowCount());
    QHashIterator<int, QVariant> it(*hash_);
    PI_VALUES values_;
    while (it.hasNext()) {
        it.next();
        values_ = it.value().value<PI_VALUES>();
        QList<QStandardItem*> row;
        QStandardItem* key = new QStandardItem();
        key->setData("0x" + QString::number(it.key(),16), Qt::DisplayRole);
        key->setData(it.key(), Qt::ToolTipRole);
        QString sValue_ = "Type/Subtype: " + QString::number(values_.type_key) + ", PI1=" + QString::number(values_.PI1_VAL) + ", PI=" + QString::number(values_.PI2_VAL);
        QStandardItem* value = new QStandardItem(sValue_);
        row << key << value;
        model_->insertRow(0, row);
    }
    model_->sort(0, Qt::AscendingOrder);
    updateInfoText();
}

void
TranslationViewer::updatePICList(QHash<int, QVariant>* hash_, QStandardItemModel* model_)
{
    model_->removeRows(0, model_->rowCount());
    QHashIterator<int, QVariant> it(*hash_);
    PIC_VALUES values_;
    while (it.hasNext()) {
        it.next();
        values_ = it.value().value<PIC_VALUES>();
        QList<QStandardItem*> row;
        QStandardItem* key = new QStandardItem();
        key->setData("0x" + QString::number(it.key(),16), Qt::DisplayRole);
        key->setData(it.key(), Qt::ToolTipRole);
        QString sValue_ = "PI1 offset:" + QString::number(values_.PI1_offset) + ", width" + QString::number(values_.PI1_width) + " | PI2 offset:" + QString::number(values_.PI2_offset) + ", width" + QString::number(values_.PI2_width);
        QStandardItem* text_ = new QStandardItem(sValue_);
        row << key << text_;
        model_->insertRow(0, row);
    }
    model_->sort(0, Qt::AscendingOrder);
    updateInfoText();
}

void
TranslationViewer::reload()
{
    updateList(myEventTranslator->getList(), this->eventListModel);
    updateList(myObjectTranslator->getList(), this->objectListModel);
    updateList(mySPIDTranslator->getList(), this->SPIDListModel);
    updatePIList(myPITranslator->getList(), this->PI_VALUESListModel);
    updatePICList(myPICTranslator->getList(), this->PIC_VALUESListModel);
}

void
TranslationViewer::comboBoxSelected(int listIndex_)
{
    switch(listIndex_) {
    case EventListIndex:
        selectedProxy = eventListModelProxy;
        break;
    case ObjectListIndex:
        selectedProxy = objectListModelProxy;
        break;
    case SPIDListIndex:
        selectedProxy = SPIDListModelProxy;
        break;
    case PI_VALUESListIndex:
        selectedProxy = PI_VALUESListModelProxy;
        break;
    case PIC_VALUESListIndex:
        selectedProxy = PIC_VALUESListModelProxy;
        break;
    }
    ui->tableView->setModel(selectedProxy);
    updateInfoText();
}

void
TranslationViewer::updateInfoText()
{
    ui->label->setText(QString::number(selectedProxy->rowCount()) + " Translation(s) found.");
}

void
TranslationViewer::addTranslation()
{
    AddTranslationDialog* dialog = new AddTranslationDialog(this, pMainWindow, ui->comboBox->currentIndex());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void
TranslationViewer::table_item_right_click(QPoint pos)
{
    QModelIndex index=ui->tableView->indexAt(pos);

    if (index.column() == 0) {
        QMenu* menu=new QMenu(this);
        QAction* hex_dec_switch = new QAction("Hex/Dec Display", this);
        hex_dec_switch->setData(index);
        menu->addAction(hex_dec_switch);

        connect(hex_dec_switch, SIGNAL(triggered()), this, SLOT(switch_hex_dec_action()));

        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void
TranslationViewer::clearTranslationTable() {
    switch(ui->comboBox->currentIndex()) {
    case EventListIndex:
        myEventTranslator->clear();
        break;
    case ObjectListIndex:
        myObjectTranslator->clear();
        break;
    case SPIDListIndex:
        mySPIDTranslator->clear();
        break;
    case PI_VALUESListIndex:
        myPITranslator->clear();
        break;
    case PIC_VALUESListIndex:
        myPICTranslator->clear();
        break;
    }
    reload();
}

void
TranslationViewer::switch_hex_dec_action()
{
    // This could be used to determine the item that was clicked...
//    QAction* pAction = qobject_cast<QAction*>(sender());
//    QModelIndex clicked_item_index = pAction->data().toModelIndex();

    for (int i=0; i<selectedProxy->sourceModel()->rowCount(); ++i) {
        // Switch data format
        QModelIndex index = selectedProxy->sourceModel()->index(i, 0);
        QString temp1 = selectedProxy->sourceModel()->data(index, Qt::DisplayRole).toString();
        QString temp2 = selectedProxy->sourceModel()->data(index, Qt::ToolTipRole).toString();
        selectedProxy->sourceModel()->setData(index, temp2, Qt::DisplayRole);
        selectedProxy->sourceModel()->setData(index, temp1, Qt::ToolTipRole);
    }
}
