#include "views/packetcontentview.h"
#include "ui_packetcontentview.h"
#include "packets/sourcepacket.h"
#include "packets/dumpsummarypacket.h"
#include "packets/variablepacket.h"
#include <QSettings>
#include <QDebug>

extern QSettings settings;

PacketContentView::PacketContentView(QWidget *parent, PacketStore *st_, qulonglong idx_) :
    QDialog(parent),
    ui(new Ui::PacketContentView),
    store(st_)
{
    ui->setupUi(this);

    selectedPacket = NULL;
    if (store != NULL) {
        // Find the selected packet
        selectedPacket = store->getPacket(idx_);
    }

    if (selectedPacket!=NULL) {
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
            QTextStream(&ssc_summary_) << "Dump Summary Packet of 0x" << QString::number(ds_packet->getOnboardStoreObject_id(), 16) << " (" << ds_packet->getObject_name() << ")" << endl;
            QTextStream(&ssc_summary_) << "Dump-ID=" << ds_packet->getDumpid() << endl;

            QHashIterator<uint32_t, bool> it(ds_packet->getL_sequencecounts());
            QTextStream(&ssc_summary_) << "\nMissing " << ds_packet->getNumberOfMissingSSC() << " packets:" << endl;
            while (it.hasNext()) {
                it.next();
                if(it.value() == false) {
                    QTextStream(&ssc_summary_) << (it.key() & 0xFFFF) << endl;
                }
            }
            QTextStream(&ssc_summary_) << "\nFound " << ds_packet->getNumberOfFoundSSC() << " packets:" << endl;
            it.toFront();
            while (it.hasNext()) {
                it.next();
                if(it.value() == true) {
                    QTextStream(&ssc_summary_) << (it.key() & 0xFFFF) << endl;
                }
            }
            ui->data_line_edit->setText(ssc_summary_);
        } else if (selectedPacket->getDataFieldHeader()->getServiceType() == 206) {
            // Variable Packet
            VariablePacket* v_packet = new VariablePacket(*selectedPacket);

            QString vp_summary_;
            QTextStream(&vp_summary_) << "Variable Packet:" << endl;
            QTextStream(&vp_summary_) << "Parameter Object ID = 0x" << QString::number(v_packet->getParr_obj_id(), 16) << endl;
            QTextStream(&vp_summary_) << "Parameter Module ID = 0x" << QString::number(v_packet->getParr_module_id(), 16) << endl;
            QTextStream(&vp_summary_) << "Parameter Array ID = 0x" << QString::number(v_packet->getParr_array_id(), 16) << endl;
            QTextStream(&vp_summary_) << "Parameter Index = 0x" << QString::number(v_packet->getParr_index(), 16) << endl;

            QTextStream(&vp_summary_) << "\n Data:" << endl;
            for (int i = 0; i < v_packet->getRows(); i++) {
                for (int j = 0; i < v_packet->getColumns(); i++) {
                    QVariant value_ = v_packet->getValues()[i + j];
                    if (value_.canConvert<int>()) {
                        QTextStream(&vp_summary_) << QString::number(value_.toInt(), 16);
                    } else if (value_.canConvert<float>()) {
                        QTextStream(&vp_summary_) << QString::number(value_.toInt(), 16);
                    } else if (value_.canConvert<QString>()) {
                        QTextStream(&vp_summary_) << QString::number(value_.toFloat());
                    } else {
                        QTextStream(&vp_summary_) << "NaN";
                    }
                    QTextStream(&vp_summary_) << "   " << endl;
                }
                QTextStream(&vp_summary_) << "\n";
            }
            ui->data_line_edit->setText(vp_summary_);
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
