#include "translationviewer.h"
#include "ui_translationviewer.h"

TranslationViewer::TranslationViewer(QWidget *parent, EventStore* event_store_) :
    QDialog(parent),
    ui(new Ui::TranslationViewer)
{
    ui->setupUi(this);

    connect(ui->pushButton_2, SIGNAL(clicked(bool)), event_store_, SLOT(loadTranslationTable()));

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

    QHashIterator<QString, QString> it(event_store_->l_event_names);
    while (it.hasNext()) {
        it.next();
        QList<QStandardItem*> row;
        row << new QStandardItem(it.key()) << new QStandardItem(it.value());
        this->eventListModel->insertRow(0, row);
    }

    QHashIterator<QString, QString> it2(event_store_->l_object_names);
    while (it2.hasNext()) {
        it2.next();
        QList<QStandardItem*> row;
        row << new QStandardItem(it2.key()) << new QStandardItem(it2.value());
        this->objectListModel->insertRow(0, row);
    }

    selectedModel = eventListModel;
    ui->tableView->setModel(selectedModel);

    updateInfoText();
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
