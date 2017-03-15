#include "checksumview.h"
#include "ui_checksumview.h"
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
              QStandardItem *checksumaddresspair = new QStandardItem(QString("Address: %0, Checksum: %1").arg(address).arg(crc));
              this->model->appendRow(checksumaddresspair);
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
