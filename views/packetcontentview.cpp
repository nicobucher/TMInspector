#include "views/packetcontentview.h"
#include "ui_packetcontentview.h"
#include "packets/sourcepacket.h"
#include <QDebug>

PacketContentView::PacketContentView(QWidget *parent, PacketStore *st_, int idx_) :
    QDialog(parent),
    ui(new Ui::PacketContentView),
    store(st_)
{
    ui->setupUi(this);

    // Find the selected packet
    selectedPacket = store->getPacket(idx_);

    if (selectedPacket!=0) {
        ui->ssc_label->setText(QString::number(selectedPacket->getSourceSequenceCount()));
        ui->type_label->setText(QString::number(selectedPacket->getSourcePacketType()));
        ui->version_label->setText(QString::number(selectedPacket->getVersion()));
        ui->apid_label->setText(QString::number(selectedPacket->getApid()));
        ui->dfh_label->setText(QString::number(selectedPacket->hasDataFieldHeader()));
        ui->sequence_label->setText(QString::number(selectedPacket->getSequence()));
        ui->bytes_label->setText(QString::number(selectedPacket->getDataLength()+1) + " byte(s)");
        ui->PI1_value->setText(QString::number(selectedPacket->getPI_1()));
        ui->PI2_value->setText(QString::number(selectedPacket->getPI_2()));
        ui->SPID->setText(QString::number(selectedPacket->getSpid()));

        ui->data_line_edit->setText(selectedPacket->getData().toHex());
    } else {
        qDebug() << "Selected Packet not found";
        ui->data_line_edit->setText("packet not found, no data");
    }
}

PacketContentView::~PacketContentView()
{
    delete ui;
}
