#ifndef ADDTRANSLATIONDIALOG_H
#define ADDTRANSLATIONDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class AddTranslationDialog;
}

class AddTranslationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTranslationDialog(QWidget *parent = 0, MainWindow* mw_ = 0, int index_ = 0);
    ~AddTranslationDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddTranslationDialog *ui;
    MainWindow* pMainWindow;
    int list_index;
};

#endif // ADDTRANSLATIONDIALOG_H
