#include "views/packetcontentview.h"
#include "ui_packetcontentview.h"
#include "packets/sourcepacket.h"
#include "packets/dumpsummarypacket.h"
#include "packets/service206.h"
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
        ui->PI1_value->setText(QString::number(selectedPacket->getPI_1()));
        ui->PI2_value->setText(QString::number(selectedPacket->getPI_2()));
        ui->SPID->setText(QString::number(selectedPacket->getSpid()));
        ui->mib->setText(selectedPacket->getMibVersion());
        ui->core->setText(QString::number(selectedPacket->getDataFieldHeader()->getCoreId()));
        ui->bootcnt->setText(QString::number(selectedPacket->getDataFieldHeader()->getBootCount()));
        ui->timestamp->setText(selectedPacket->getDataFieldHeader()->getTimestampString(settings.value("time_fmt").toString()));

        if (selectedPacket->getDataFieldHeader()->getServiceType() == 15 && selectedPacket->getDataFieldHeader()->getSubServiceType() == 128) {
            // Special view for dump summary packets
            DumpSummaryPacket* ds_packet = new DumpSummaryPacket(*selectedPacket);
            ds_packet->checkPackets();
            QString ssc_summary_;
            QTextStream(&ssc_summary_) << "Dump Summary Packet of 0x" << QString::number(ds_packet->getOnboardStoreObject_id(), 16) << " (" << ds_packet->getObject_name() << ")" << endl;
            QTextStream(&ssc_summary_) << "Dump-ID=" << ds_packet->getDumpid() << endl;

            QHashIterator<uint32_t, bool> it(ds_packet->getL_sequencecounts());
            QTextStream(&ssc_summary_) << "\nMissing " << ds_packet->getNumberOfMissingSSC() << " packets:" << endl;
            while (it.hasNext()) {
                it.next();
                if(it.value() == false) {
                    QTextStream(&ssc_summary_) << (it.key() & 0xFFFF) << " (APID: " << ((it.key() & 0xFFFF0000) >> 16) << endl;
                }
            }
            QTextStream(&ssc_summary_) << "\nFound " << ds_packet->getNumberOfFoundSSC() << " packets:" << endl;
            it.toFront();
            while (it.hasNext()) {
                it.next();
                if(it.value() == true) {
                    QTextStream(&ssc_summary_) << (it.key() & 0xFFFF) << " (APID: " << ((it.key() & 0xFFFF0000) >> 16) << endl;
                }
            }
            ui->data_line_edit->setText(ssc_summary_);
        } else if (selectedPacket->getDataFieldHeader()->getServiceType() == 206) {
            // Variable Packet
            Service206* v_packet = new Service206(*selectedPacket);

            QString vp_summary_;
            QTextStream text(&vp_summary_);
            text << "Variable Packet:" << endl;
            text << "Parameter Object ID = 0x" << QString::number(((VariablePacket*)v_packet)->getParr_obj_id(), 16) << endl;
            text << "Parameter Module ID = 0x" << QString::number(v_packet->getParr_module_id(), 16) << endl;
            text << "Parameter Array ID = 0x" << QString::number(v_packet->getParr_array_id(), 16) << endl;
            text << "Parameter Index = 0x" << QString::number(v_packet->getParr_index(), 16) << endl;

            text << "\n Data:" << endl;
            for (int i = 0; i < v_packet->getRows(); i++) {
                text << qSetFieldWidth(0) << "\n";
                for (int j = 0; j < v_packet->getColumns(); j++) {
                    QVariant value_ = ((VariablePacket*)v_packet)->getValues()[i + j];
                    if (value_.type() == QMetaType::Float || value_.type() == QMetaType::Double) {
                        text << qSetFieldWidth(16) << left << fixed << QString::number(value_.toFloat());
                    } else if (value_.canConvert<int>()) {
                        text << "0x" << qSetFieldWidth(16) << left << QString::number(value_.toInt(), 16);
                    } else if (value_.canConvert<QString>()) {
                        text << qSetFieldWidth(0) << value_.toString();
                    } else {
                        text << "NaN";
                    }
                }
            }
            ui->data_line_edit->setText(vp_summary_);
            // TODO: This is not quite right for a variable packet the structure is also defined in MIB.. Which packets can be assumed to have similar structure though?
//        } else if (selectedPacket->getDataFieldHeader()->getServiceType() == 12 && selectedPacket->getDataFieldHeader()->getSubServiceType() == 12) {
//            // Variable Packet
//            VariablePacket* v_packet = new VariablePacket(*selectedPacket);

//            QString vp_summary_;
//            QTextStream text(&vp_summary_);
//            text << "Variable Packet:" << endl;
//            text << "Parameter Object ID = 0x" << QString::number(v_packet->getParr_obj_id(), 16) << endl;

//            text << "\n Data:" << endl;
//            for (int i = 0; i < v_packet->getValues().size(); i++) {
//                text << qSetFieldWidth(0) << "\n";
//                QVariant value_ = v_packet->getValues()[i];
//                if (value_.type() == QMetaType::Float || value_.type() == QMetaType::Double) {
//                    text << qSetFieldWidth(16) << left << fixed << QString::number(value_.toFloat());
//                } else if (value_.canConvert<int>()) {
//                    text << "0x" << qSetFieldWidth(16) << left << QString::number(value_.toInt(), 16);
//                } else if (value_.canConvert<QString>()) {
//                    text << qSetFieldWidth(0) << value_.toString();
//                } else {
//                    text << "NaN";
//                }
//            }
//            ui->data_line_edit->setText(vp_summary_);
        } else {
            QString packet_text;
            QTextStream text(&packet_text);
            text << "Raw Content (" << selectedPacket->getDataLength() + 1 << " bytes):\n" << endl;
            text << selectedPacket->getData().toHex();
            ui->data_line_edit->setText(packet_text);
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
