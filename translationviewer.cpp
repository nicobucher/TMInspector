#include "translationviewer.h"
#include "ui_translationviewer.h"
#include "addtranslationdialog.h"
#include <QMenu>

TranslationViewer::TranslationViewer(QWidget *parent, QHash<QString,QString>* l_objn_, QHash<QString,QString>* l_evn_, QHash<QString,QString>* l_pac_, QHash<int,PI_VALUES>* l_spids_, QHash<int,PIC_VALUES>* l_types_) :
    QDialog(parent),
    ui(new Ui::TranslationViewer),
    l_object_names(l_objn_),
    l_event_names(l_evn_),
    l_packet_names(l_pac_),
    l_spids(l_spids_),
    l_types(l_types_)
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
TranslationViewer::updateList(QHash<QString, QString>* hash_, QStandardItemModel* model_)
{
    model_->removeRows(0, model_->rowCount());
    QHashIterator<QString, QString> it(*hash_);
    while (it.hasNext()) {
        it.next();
        QList<QStandardItem*> row;
        QStandardItem* key = new QStandardItem();
        key->setData("0x" + QString::number(it.key().toInt(),16), Qt::DisplayRole);
        key->setData(it.key(), Qt::ToolTipRole);
        QStandardItem* value = new QStandardItem(it.value());
        row << key << value;
        model_->insertRow(0, row);
    }
    model_->sort(0, Qt::AscendingOrder);
    updateInfoText();
}

void
TranslationViewer::updateList(QHash<int, PI_VALUES>* hash_, QStandardItemModel* model_)
{
    model_->removeRows(0, model_->rowCount());
    QHashIterator<int, PI_VALUES> it(*hash_);
    while (it.hasNext()) {
        it.next();
        QList<QStandardItem*> row;
        QStandardItem* key = new QStandardItem();
        key->setData("0x" + QString::number(it.key(),16), Qt::DisplayRole);
        key->setData(it.key(), Qt::ToolTipRole);
        QString sValue_ = QString::number(it.value().PI1_VAL) + ", " + QString::number(it.value().PI2_VAL);
        QStandardItem* value = new QStandardItem(sValue_);
        row << key << value;
        model_->insertRow(0, row);
    }
    model_->sort(0, Qt::AscendingOrder);
    updateInfoText();
}

void
TranslationViewer::updateList(QHash<int, PIC_VALUES>* hash_, QStandardItemModel* model_)
{
    model_->removeRows(0, model_->rowCount());
    QHashIterator<int, PIC_VALUES> it(*hash_);
    while (it.hasNext()) {
        it.next();
        QList<QStandardItem*> row;
        QStandardItem* key = new QStandardItem();
        key->setData("0x" + QString::number(it.key(),16), Qt::DisplayRole);
        key->setData(it.key(), Qt::ToolTipRole);
        QString sValue_ = "PI1 offset:" + QString::number(it.value().PI1_offset) + ", width" + QString::number(it.value().PI1_width) + "PI2 offset:" + QString::number(it.value().PI2_offset) + ", width" + QString::number(it.value().PI2_width);
        QStandardItem* value = new QStandardItem(sValue_);
        row << key << value;
        model_->insertRow(0, row);
    }
    model_->sort(0, Qt::AscendingOrder);
    updateInfoText();
}

void
TranslationViewer::reload()
{
    updateList(l_event_names, this->eventListModel);
    updateList(l_object_names, this->objectListModel);
    updateList(l_packet_names, this->SPIDListModel);
    updateList(l_spids, this->PI_VALUESListModel);
    updateList(l_types, this->PIC_VALUESListModel);
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
        l_event_names->clear();
        break;
    case ObjectListIndex:
        l_object_names->clear();
        break;
    case SPIDListIndex:
        l_packet_names->clear();
        break;
    case PI_VALUESListIndex:
        l_spids->clear();
        break;
    case PIC_VALUESListIndex:
        l_types->clear();
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
