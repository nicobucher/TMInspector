#include "serversettings.h"
#include "ui_serversettings.h"

ServerSettings::ServerSettings(QWidget *parent, QSettings* pSettings) :
    QDialog(parent),
    ui(new Ui::ServerSettings),
    settings(pSettings)
{
    ui->setupUi(this);
    ui->lineEdit->setText(settings->value("server/host").toString());
    ui->lineEdit_2->setText(settings->value("server/port").toString());

    ui->server->setText(settings->value("db/host").toString());
    ui->port->setText(settings->value("db/port").toString());
    ui->db->setText(settings->value("db/database").toString());
    ui->user->setText(settings->value("db/username").toString());

    ui->pw->setEchoMode(QLineEdit::Password);
    ui->pw->setText(settings->value("db/pw").toString());
}

ServerSettings::~ServerSettings()
{
    delete ui; 
}

void ServerSettings::accept()
{
    settings->setValue("server/host", ui->lineEdit->text());
    settings->setValue("server/port", ui->lineEdit_2->text().toInt());

    settings->setValue("db/host", ui->server->text());
    settings->setValue("db/port", ui->port->text().toInt());
    settings->setValue("db/database", ui->db->text());
    settings->setValue("db/username", ui->user->text());
    settings->setValue("db/pw", ui->pw->text());

    this->QDialog::accept();
}
