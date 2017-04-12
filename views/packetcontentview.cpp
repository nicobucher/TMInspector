#include "views/packetcontentview.h"
#include "ui_packetcontentview.h"
#include "packets/sourcepacket.h"
#include "packets/dumpsummarypacket.h"
#include <QSettings>
#include <QDebug>

extern QSettings settings;

PacketContentView::PacketContentView(QWidget *parent, PacketStore *st_, qulonglong idx_) :
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
        ui->mib->setText(selectedPacket->getMibVersion());
        ui->core->setText(QString::number(selectedPacket->getDataFieldHeader()->getCoreId()));
        ui->bootcnt->setText(QString::number(selectedPacket->getDataFieldHeader()->getBootCount()));
        ui->timestamp->setText(selectedPacket->getDataFieldHeader()->getTimestampString(settings.value("time_fmt").toString()));

        if (selectedPacket->getDataFieldHeader()->getServiceType() == 15 && selectedPacket->getDataFieldHeader()->getSubServiceType() == 128) {
            // Special view for dump summary packets
            DumpSummaryPacket* ds_packet = (DumpSummaryPacket*)selectedPacket;
            QString ssc_summary_;
            QTextStream(&ssc_summary_) << "Dump Summary Packet (Dump-ID=" << ds_packet->getDumpid() << ")" << endl;
            QTextStream(&ssc_summary_) << "Missing " << ds_packet->getNumberOfMissingSSC() << " packets:" << endl;
            QHashIterator<uint16_t, uint16_t> it(ds_packet->getL_missing_sequencecounts());
            while (it.hasNext()) {
                it.next();
                QTextStream(&ssc_summary_) << it.key() << endl;
            }
            QTextStream(&ssc_summary_) << "\nFound " << ds_packet->getNumberOfSSC() << " packets:" << endl;
            it = QHashIterator<uint16_t, uint16_t>(ds_packet->getL_sequencecounts());
            while (it.hasNext()) {
                it.next();
                QTextStream(&ssc_summary_) << it.key() << endl;
            }
            ui->data_line_edit->setText(ssc_summary_);
        } else {
            ui->data_line_edit->setText(selectedPacket->getData().toHex());
        }

    } else {
        qDebug() << "Selected Packet not found";
        ui->data_line_edit->setText("packet not found, no data");
    }
}

PacketContentView::~PacketContentView()
{
    delete ui;
}
