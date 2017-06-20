#include "addtranslationdialog.h"
#include "ui_addtranslationdialog.h"

AddTranslationDialog::AddTranslationDialog(QWidget *parent, MainWindow *mw_, int index_) :
    QDialog(parent),
    ui(new Ui::AddTranslationDialog)
{
    ui->setupUi(this);
    pMainWindow = mw_;
    list_index = index_;
}

AddTranslationDialog::~AddTranslationDialog()
{
    delete ui;
}

void AddTranslationDialog::on_buttonBox_accepted()
{
    QString address = ui->lineEdit->text();
    bool ok;
    int key = address.toInt(&ok, 10);
    if (address[0] == '0' && address[1] == 'x') {
        key = address.remove(0,2).toInt(&ok, 16);
    }
    QString translation = ui->lineEdit_2->text();
    pMainWindow->addTranslation(key, translation, list_index);
}
