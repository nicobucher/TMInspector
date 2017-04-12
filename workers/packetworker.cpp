#include "workers/packetworker.h"
#include "packets/sourcepacket.h"
#include "packets/dumpsummarypacket.h"
#include "event.h"
#include <QMutexLocker>
#include <QDebug>
#include <QNetworkProxy>

PacketWorker::PacketWorker()
{
    socket = new QTcpSocket(this);
    socket->setProxy(QNetworkProxy::NoProxy);
    this->isReady = false;

    this->quit = false;
}

PacketWorker::~PacketWorker()
{
    this->socket->close();
}

void
PacketWorker::setup(QThread* th_, QString h_, int p_)
{
    connect(th_, SIGNAL(started()), this, SLOT(doWork()));
    connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(networkError(QAbstractSocket::SocketError)));

    this->host = h_;
    this->port = p_;

    this->socket->connectToHost(this->host, this->port);
    if (this->socket->waitForConnected(3000)) {
        qDebug() << "connected to " << this->host << " on port " << this->port;
        this->isReady = true;
    } else {
        qDebug() << "error connecting";
        this->quit = true;
        emit hasError(this->socket->errorString());
    }
}

void
PacketWorker::doWork()
{
    //QByteArray data;
    while (!quit) {
        if (this->socket->isReadable()) {
            this->socket->waitForReadyRead(1000);       // Blocks for 1 sec max.
            while (this->socket->bytesAvailable()) {
                //data = this->socket->readAll();
                QByteArray header_buffer = this->socket->read(SourcePacket::HEADER_SIZE) ;
                if (header_buffer.size() != SourcePacket::HEADER_SIZE) {
                    qDebug() << "Error reading packet-header";
                    this->quit = true;
                } else {
//                    qDebug() << "Header: " << header_buffer.toHex();
                    int data_length = ((unsigned char)header_buffer.at(4) << 8) + (unsigned char)header_buffer.at(5) + 1;
//                    qDebug() << "Reading header, packet has length " << data_length-1;
                    QByteArray data_buffer = this->socket->read(data_length);
                    if (data_buffer.size() != data_length) {
                        qDebug() << "Error reading packet-data";
                        this->quit = true;
                    } else {
                        // Create the packet
                        SourcePacket* packet = new SourcePacket();
                        packet->makePacketFromData((unsigned char*)header_buffer.data(), (unsigned char*)data_buffer.data(), data_length);

                        if (packet->getQuality() == GOOD &&
                                packet->getApid() != SourcePacket::APID_IDLEPACKET) {

                            // Dump Summary Packet
                            if (packet->getDataFieldHeader()->getServiceType() == 15 &&
                                    packet->getDataFieldHeader()->getSubServiceType() == 128) {
                                DumpSummaryPacket* ds_packet = new DumpSummaryPacket(*packet);

                                emit dumpSummaryReceived(ds_packet);
                            } else {
                                emit packetReceived(packet);
                            }

                            // If the packet contains an event (Events have Service Type 5)
                            if (packet->hasDataFieldHeader()) {
                                if (packet->getDataFieldHeader()->getServiceType() == 5) {
                                    Event* event = new Event(packet->getDataFieldHeader()->getTimestamp(), (Severity)packet->getDataFieldHeader()->getSubServiceType(), (unsigned char*)packet->getData().data());
                                    event->setPacketReference(packet->getId());
                                    // Put the event into the event store
                                    emit eventReceived(event);
                                    emit eventAdded(event);
                                }
                            }

                        } else {
                            // If the packet is either bad or an idle packet...
//                            qDebug() << data_buffer.toHex();
                        }
                    }
                }
                //qDebug() << data.toHex();
            }
        } else {
            qDebug() << "Socket not readable, quitting thread";
            this->quit = true;
        }
    }
    this->socket->abort();
    emit hasError(this->socket->errorString());
    qDebug() << "exiting thread";
}

void
PacketWorker::networkError(QAbstractSocket::SocketError e_)
{
    if (e_ == QAbstractSocket::SocketTimeoutError) {
        return;
    } else if (e_ == QAbstractSocket::ConnectionRefusedError) {
        return;
    } else {
        qDebug() << e_;
        this->quit = true;
    }
}
