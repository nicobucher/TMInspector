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
    QString key = ui->lineEdit->text();
    QString translation = ui->lineEdit_2->text();
    pMainWindow->addTranslation(key, translation, list_index);
}
