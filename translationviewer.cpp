#include "translationviewer.h"
#include "ui_translationviewer.h"

TranslationViewer::TranslationViewer(QWidget *parent, EventStore* event_store_) :
    QDialog(parent),
    ui(new Ui::TranslationViewer),
    event_store(event_store_)
{
    ui->setupUi(this);

    connect(ui->pushButton_2, SIGNAL(clicked(bool)), event_store, SLOT(loadTranslationTable()));
    connect(event_store, SIGNAL(hashUpdated()), this, SLOT(reload()));

    QStringList list;
    list << "Event-IDs" << "Object-IDs";
    ui->comboBox->addItems(list);

    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(comboBoxSelected(int)));

    eventListModel = new QStandardItemModel(this);
    objectListModel = new QStandardItemModel(this);

    list.clear();
    list << "Key" << "Value";
    eventListModel->setHorizontalHeaderLabels(list);
    objectListModel->setHorizontalHeaderLabels(list);

    selectedModel = eventListModel;
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView->setModel(selectedModel);

    reload();
    updateInfoText();
}

void
TranslationViewer::updateList(QHash<QString, QString>* hash_, QStandardItemModel* model_)
{
    if (hash_->size() > 0) {
        model_->removeRows(0, model_->rowCount());
        QHashIterator<QString, QString> it(*hash_);
        while (it.hasNext()) {
            it.next();
            QList<QStandardItem*> row;
            row << new QStandardItem(it.key()) << new QStandardItem(it.value());
            model_->insertRow(0, row);
        }
        model_->sort(0, Qt::AscendingOrder);
    }
}

void
TranslationViewer::reload()
{
    updateList(&event_store->l_event_names, this->eventListModel);
    updateList(&event_store->l_object_names, this->objectListModel);
}

void
TranslationViewer::comboBoxSelected(int listIndex_)
{
    switch(listIndex_) {
    case 0:
        selectedModel = eventListModel;
        break;
    case 1:
        selectedModel = objectListModel;
        break;
    }
    ui->tableView->setModel(selectedModel);
    updateInfoText();
}

void
TranslationViewer::updateInfoText()
{
    ui->label->setText(QString::number(selectedModel->rowCount()) + " Translation(s) found.");
}

TranslationViewer::~TranslationViewer()
{
    delete ui;
}
