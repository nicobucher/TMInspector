#include "checksumview.h"
#include "ui_checksumview.h"
#include "definitions.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>

ChecksumView::ChecksumView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Checksumview)
{
    ui->setupUi(this);
    this->model = new QStandardItemModel();
    ui->listView->setModel(this->model);

    connect(ui->loadFile, SIGNAL(clicked(bool)), this, SLOT(loadChecksumFile()));
    connect(ui->clear, SIGNAL(clicked(bool)), this, SLOT(clear()));
}

ChecksumView::~ChecksumView()
{
    delete ui;
}

bool ChecksumView::getAutoFocus()
{
    return ui->check_Box_focus->isChecked();
}

void ChecksumView::receiveChecksum(qint32 address, qint16 checksum)
{
    qulonglong pair;
    qulonglong longaddress = (qulonglong)address;
    quint16 unsigned_checksum = (quint16)checksum;
    pair = (longaddress << 32) + unsigned_checksum;
    for (int row = 0; row < this->model->rowCount(); ++row) {
        QModelIndex index = this->model->index(row, 0);
        if(longaddress == this->model->data(index, ExtraRoles::ListIndexRole)) {
            if (ui->checkBox_scroll->isChecked()) {
                // Scroll to the next row...
                ui->listView->scrollTo(this->model->index(row + 1, 0));
            }
            QString text = this->model->data(index).toString();
            text = text.append(" (received 0x%0)").arg(unsigned_checksum, 4, 16, QLatin1Char( '0' ));
            this->model->setData(index, text);
            if (pair == this->model->data(index, Qt::UserRole)) {
                this->model->setData(index, QVariant(QBrush(Qt::green)), Qt::ForegroundRole);
            }
        }
    }
}

void ChecksumView::loadChecksumFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load File","","Type (*.dat)");
    QStringList list;
    QFile inputFile(filename);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QRegExp rx("0[xX][0-9a-fA-F]+");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          int pos = 0;
          int count = 0;
          QString address = "";
          QString crc = "";
          while ((pos = rx.indexIn(line, pos)) != -1)
          {
              switch(count) {
              case 0:
                  address = rx.cap();
                  break;
              case 1:
                  crc = rx.cap();
                  break;
              default:
                  continue;
              }
              pos += rx.matchedLength();
              count++;
          }
          if (address != "" && crc != "") {
              QString text("");

              bool adr_ok;
              qulonglong adr_int = address.toInt(&adr_ok, 16);
              if (!adr_ok) {
                  text.append("Could not convert address; ");
              }
              bool crc_ok;
              quint16 crc_int = crc.toInt(&crc_ok, 16);
              if (!crc_ok) {
                  text.append("Could not convert checksum; ");
              }
              if (adr_ok && crc_ok) {
                  text.append(QString("Address: 0x%0, Checksum: 0x%1").arg(adr_int, 8, 16, QLatin1Char( '0' )).arg(crc_int, 4, 16, QLatin1Char( '0' )));
              }

              qulonglong pair;
              pair = (adr_int << 32) + crc_int;
              QStandardItem *checksumitem = new QStandardItem(text);
              this->model->appendRow(checksumitem);
              this->model->setData(this->model->indexFromItem(checksumitem), pair, Qt::UserRole);
              this->model->setData(this->model->indexFromItem(checksumitem), adr_int, ExtraRoles::ListIndexRole);
          }
       }
       inputFile.close();
       if (this->model->rowCount() == 0) {
           QStandardItem *errormessage = new QStandardItem(QString("No valid checksums found."));
           this->model->appendRow(errormessage);
       }
    } else {
        list << "Could not open file " << filename;
    }
}

void ChecksumView::clear()
{
    this->model->clear();
}
