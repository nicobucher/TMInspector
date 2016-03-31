#include "packetcontentview.h"
#include "ui_packetcontentview.h"
#include "sourcepacket.h"
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
        ui->bytes_label->setText(QString::number(selectedPacket->getDataLength()) + " byte(s)");
        ui->data_line_edit->setText(selectedPacket->getData().toHex());
    } else {
        qDebug() << "Selected Packet not found";
    }
}

PacketContentView::~PacketContentView()
{
    delete ui;
}
