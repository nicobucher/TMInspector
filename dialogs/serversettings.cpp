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

    ui->mib_db->setText(settings->value("mib/db").toString());
    ui->mib_user->setText(settings->value("mib/user").toString());

    ui->pw->setEchoMode(QLineEdit::Password);
    ui->mib_pw->setEchoMode(QLineEdit::Password);
    ui->pw->setText(settings->value("db/pw").toString());
    ui->mib_pw->setText(settings->value("mib/pw").toString());

    ui->time_fmt->setText(settings->value("time_fmt").toString());
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

    settings->setValue("mib/db", ui->mib_db->text());
    settings->setValue("mib/user", ui->mib_user->text());
    settings->setValue("mib/pw", ui->mib_pw->text());

    settings->setValue("time_fmt", ui->time_fmt->text());

    this->QDialog::accept();
}
