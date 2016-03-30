#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objectview.h"
#include "translationviewer.h"
#include "sqlworker.h"
#include <QTableView>
#include <QDateTime>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add the Connect Menu Entry
    dataMenu = menuBar()->addMenu("Data");
    action_Connect = dataMenu->addAction("Connect");
    action_Connect->setShortcut(Qt::Key_F10);
    connect(action_Connect, SIGNAL(triggered()), this, SLOT(on_actionTo_Server_triggered()));
    // Add the Switch to Event Mode Menu Entry and set the shortcut key to be Tab
    dataMenu->addAction("Settings", this, SLOT(on_actionEdit_triggered()));
    action_EventMode = dataMenu->addAction("Event Mode");
    action_EventMode->setCheckable(true);
    action_EventMode->setShortcut(Qt::Key_Tab);
    connect(action_EventMode, SIGNAL(triggered()), this, SLOT(eventMode_triggered()));
    // Add Export Menu Item
    dataMenu->addAction("Export", this, SLOT(exportTriggered()));

    statusLabel = new QLabel(this);
    statusLabel->setText("Disconneted");
    this->statusBar()->addPermanentWidget(statusLabel);

    myPacketWorker = 0;
    myPacketWorkerThread = 0;

    myPacketStore = new PacketStore(this);
    mySqlPacketStore = new PacketStore(this);
    myEventStore = new EventStore(this, settings);
    mySqlEventStore = new EventStore(this, settings);

    dataMenu->addAction("Translation Table", this, SLOT(translation_triggered()));

    QCoreApplication::setOrganizationName("Institute of Space Systems");
    QCoreApplication::setOrganizationDomain("www.irs.uni-stuttgart.de");
    QCoreApplication::setApplicationName("TMInspector");

    settings = new QSettings();
    readSettings();
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView_arch->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->setRootIsDecorated(true);
    ui->treeView_arch->setRootIsDecorated(true);
    ui->treeView->setUniformRowHeights(true);   // <- should increase performance
    ui->treeView_arch->setUniformRowHeights(true);   // <- should increase performance
    // todo: this can be enabled if required...
//    ui->treeView->setAlternatingRowColors(true);
//    ui->treeView->setSortingEnabled(true);
    if (settings->value("ui/eventmode").toBool()) {
        ui->treeView->setModel(myEventStore->proxy_model);
        ui->treeView_arch->setModel(mySqlEventStore->proxy_model);
        action_EventMode->setChecked(true);
    } else {
        ui->treeView_arch->setModel(mySqlPacketStore->proxy_model);
        ui->treeView->setModel(myPacketStore->proxy_model);
    }

    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadObjectView(QModelIndex)));
    connect(ui->treeView_arch, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadObjectView_Arch(QModelIndex)));

    // Initialize the DateTime Pickers
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_stop->setCalendarPopup(true);
    QDateTime now = QDateTime::currentDateTime();
    ui->dateTimeEdit_start->setDate(now.date());
    ui->dateTimeEdit_stop->setDateTime(now);


    /* Initial Data for both models (for testing ->>*/
    SourcePacket test_packet(0, 3, 53);
    myPacketStore->putPacket(&test_packet);

    Event testevent(now, (Severity)1);
    testevent.setEventId(1);
    testevent.setObjectId(1000);
    testevent.setObjectName("Test-Event");
    testevent.setParams(23, 24);
    myEventStore->putEvent(&testevent);

    Event testevent1(now, (Severity)2);
    testevent1.setEventId(2);
    testevent1.setObjectId(1000);
    testevent1.setObjectName("Bla");
    testevent1.setParams(23, 24);
    myEventStore->putEvent(&testevent1);

    Event testevent2(now, (Severity)3);
    testevent2.setEventId(3);
    testevent2.setObjectId(1000);
    testevent2.setObjectName("Engä");
    testevent2.setParams(23, 24);
    myEventStore->putEvent(&testevent2);

    /* <<- remove */

    // The RegEx Filters for the EventStores
    connect(ui->lineEdit_3, SIGNAL(textChanged(QString)),myEventStore->proxy_model, SLOT(setFilterRegExp(QString)));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)),mySqlEventStore->proxy_model, SLOT(setFilterRegExp(QString)));

    connect(myPacketWorker, SIGNAL(eventAdded(Event*)), this, SLOT(animateNewEvent(Event*)));
}

MainWindow::~MainWindow()
{
    if (myPacketWorkerThread != 0) {
        // Signal the packet worker object to quit, then quit the packet worker thread and wait for it to finish
        myPacketWorker->quit = true;
        myPacketWorkerThread->quit();
        myPacketWorkerThread->wait();
        delete myPacketWorkerThread;
    }

    if (myPacketWorker != 0) {
        delete myPacketWorker;
    }

    delete myPacketStore;
    delete myEventStore;
    delete mySqlEventStore;
    delete mySqlPacketStore;
    delete settings;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::on_actionTo_Server_triggered()
{
    this->statusBar()->clearMessage();

    if (action_Connect->text() == "Disconnect") {
        if (myPacketWorkerThread != 0) {
            // Signal the packet worker object to quit, then quit the packet worker thread and wait for it to finish
            myPacketWorker->quit = true;
        }
        statusLabel->setText(tr("Disconnected"));
        action_Connect->setText("Connect");
        return;
    }

    myPacketWorker = new PacketWorker(myPacketStore, myEventStore);
    connect(myPacketWorker, SIGNAL(hasError(const QString&)), this, SLOT(displayPacketWorkerError(const QString&)));
    connect(this, SIGNAL(clientSetup(QThread*,QString,int)), myPacketWorker, SLOT(setup(QThread*,QString,int)));
    myPacketWorkerThread = new QThread();

    emit clientSetup(myPacketWorkerThread, settings->value("server/host").toString(), settings->value("server/port").toInt());
    if (myPacketWorkerThread != 0 && myPacketWorker != 0) {
        if (myPacketWorker->isReady) {
            myPacketWorker->moveToThread(myPacketWorkerThread);
            myPacketWorkerThread->start();
            statusLabel->setText(tr("Connected"));
            action_Connect->setText("Disconnect");
        } else {
            QString str;
            QTextStream(&str) << "Could not connect to " << settings->value("server/host").toString() << " on port " << settings->value("server/port").toInt();
            this->statusBar()->showMessage(str);
        }
    }
}

void MainWindow::writeSettings()
{
    settings->beginGroup("MainWindow");
    settings->setValue("size", size());
    settings->setValue("pos", pos());
    settings->endGroup();
}

void MainWindow::readSettings()
{
    settings->beginGroup("MainWindow");
    resize(settings->value("size", QSize(400, 400)).toSize());
    move(settings->value("pos", QPoint(200, 200)).toPoint());
    settings->endGroup();
}

void MainWindow::on_actionEdit_triggered()
{
    serverSettingsWindow = new ServerSettings(this, settings);
    serverSettingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    serverSettingsWindow->show();
    serverSettingsWindow->raise();
    serverSettingsWindow->activateWindow();
}

void MainWindow::displayStatusBarMessage(const QString message)
{
    this->statusBar()->showMessage(message);
}

void MainWindow::displayPacketWorkerError(const QString errormessage)
{
    this->statusBar()->showMessage(errormessage);
    qDebug() << "deleting thread";
    myPacketWorkerThread->quit();
    myPacketWorkerThread->wait();
    delete myPacketWorkerThread;
    delete myPacketWorker;
    myPacketWorkerThread = 0;
    myPacketWorker = 0;
    statusLabel->setText(tr("Disconnected"));
    action_Connect->setText("Connect");
}

void MainWindow::on_commandLinkButton_clicked()
{
    SqlWorker worker(qobject_cast<QObject*>(this), settings);
    QDateTime begin_ = ui->dateTimeEdit_start->dateTime();
    QDateTime end_ = ui->dateTimeEdit_stop->dateTime();

    QList<SourcePacket*> retrievedPackets;
    retrievedPackets = worker.fetchPackets(begin_, end_);
    if (retrievedPackets.size() > 0) {
        mySqlPacketStore->emptyStore();
        unsigned char* complete_packet_data = (unsigned char*) malloc(SourcePacket::MAX_PACKET_SIZE); // Maximum TM packet size
        for (int i = 0; i < retrievedPackets.size(); ++i) {
            SourcePacket* packet = retrievedPackets.at(i);
            mySqlPacketStore->putPacket(packet);

            if (packet->hasDataFieldHeader()) {
                if (packet->getDataFieldHeader()->getServiceType() == 5) {
                    Event* event = new Event(packet->getDataFieldHeader()->getTimestamp(), (Severity)packet->getDataFieldHeader()->getSubServiceType());
                    int data_length = packet->getDataLength();
                    if ( data_length > 0 && data_length < SourcePacket::MAX_PACKET_SIZE ) {
                        memcpy(complete_packet_data+12, packet->getData(), packet->getDataLength());
                        event->makeEventfromPacketData(complete_packet_data);
                    }
                    // Put the event into the event store
                    mySqlEventStore->putEvent(event);
                }
            }
        }
        free(complete_packet_data);
    }
}

void MainWindow::eventMode_triggered()
{
    if (!action_EventMode->isChecked()) {
        ui->treeView->setModel(myPacketStore->proxy_model);
        ui->treeView_arch->setModel(mySqlPacketStore->proxy_model);
        settings->setValue("ui/eventmode", false);
    } else {
        ui->treeView->setModel(myEventStore->proxy_model);
        ui->treeView_arch->setModel(mySqlEventStore->proxy_model);
        settings->setValue("ui/eventmode", true);
    }
}

void MainWindow::loadObjectView(QModelIndex index)
{
    if (myEventStore->itemInStore(index.data().toString())) {
        // The mapping to the source model is required because index is of the proxy_model and needs to be mapped to the source model in order to be resolved
        QModelIndex sourceIndex = myEventStore->proxy_model->mapToSource(index);
        // Then pass the mapped sourceIndex to the ObjectView
        ObjectView* objView = new ObjectView(this, sourceIndex, myEventStore->model);
        objView->setAttribute(Qt::WA_DeleteOnClose);
        objView->show();
        objView->raise();
        objView->activateWindow();
    }
}

void MainWindow::loadObjectView_Arch(QModelIndex index)
{
    if (mySqlEventStore->itemInStore(index.data().toString())) {
        // The mapping to the source model is required because index is of the proxy_model and needs to be mapped to the source model in order to be resolved
        QModelIndex sourceIndex = mySqlEventStore->proxy_model->mapToSource(index);
        // Then pass the mapped sourceIndex to the ObjectView
        ObjectView* objView = new ObjectView(this, sourceIndex, mySqlEventStore->model);
        objView->setAttribute(Qt::WA_DeleteOnClose);
        objView->show();
        objView->raise();
        objView->activateWindow();
    }
}

void MainWindow::translation_triggered()
{
    TranslationViewer* transView = new TranslationViewer(this, myEventStore);
    transView->setAttribute(Qt::WA_DeleteOnClose);
    transView->show();
    transView->raise();
    transView->activateWindow();
}

void MainWindow::exportTriggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save File",
                               "",
                               "Type (*.csv)");
    if (!action_EventMode->isChecked()) {
        myPacketStore->exportToFile(filename);
    } else {
        myEventStore->exportToFile(filename);
    }
}

void MainWindow::animateNewEvent(Event* event)
{
    QDateTime now = QDateTime::currentDateTime();
    if (event->getTimestamp().secsTo(now) < 60*60) { // If the event was generated within the last hour
        event->getSeverityItem()->animate();
    }
}
