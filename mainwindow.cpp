#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objectview.h"
#include "translationviewer.h"
#include "sqlworker.h"
#include <QTableView>
#include <QDateTime>

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

    statusLabel = new QLabel(this);
    statusLabel->setText("Disconneted");
    statusBar()->addPermanentWidget(statusLabel);

    myPacketWorker = 0;
    myPacketWorkerThread = 0;

    myPacketStore = new PacketStore(this);
    myEventStore = new EventStore(this);

    dataMenu->addAction("Translation Table", this, SLOT(translation_triggered()));

    QCoreApplication::setOrganizationName("Institute of Space Systems");
    QCoreApplication::setOrganizationDomain("www.irs.uni-stuttgart.de");
    QCoreApplication::setApplicationName("TMInspector");

    settings = new QSettings();
    readSettings();
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->setRootIsDecorated(true);
    ui->treeView->setUniformRowHeights(true);   // <- should increase performance
    // todo: this can be enabled if required...
//    ui->treeView->setAlternatingRowColors(true);
//    ui->treeView->setSortingEnabled(true);
    if (settings->value("ui/eventmode").toBool()) {
        ui->treeView->setModel(myEventStore->model);
        action_EventMode->setChecked(true);
    } else {
        ui->treeView->setModel(myPacketStore->model);
    }

    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadObjectView(QModelIndex)));

    // Initialize the DateTime Pickers
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_stop->setCalendarPopup(true);
    QDateTime now = QDateTime::currentDateTime();
    ui->dateTimeEdit_start->setDate(now.date());
    ui->dateTimeEdit_stop->setDateTime(now);


    /* Initial Data for both models (for testing */
    SourcePacket test_packet(0, 3, 53);
    myPacketStore->putPacket(&test_packet);

    Event testevent(now, (Severity)1);
    testevent.setEventId(1);
    testevent.setObjectId(1000);
    testevent.setObjectName("Test-Event");
    testevent.setParams(23, 24);
    myEventStore->putEvent(&testevent);
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
    statusBar()->clearMessage();

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
    connect(myPacketWorker, SIGNAL(hasError(const QString&)), this, SLOT(displayError(const QString&)));
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
            statusBar()->showMessage(str);
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

void MainWindow::displayError(const QString errormessage)
{
    statusBar()->showMessage(errormessage);
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
    SqlWorker worker(settings->value("db/username").toString(),
                     settings->value("db/host").toString(),
                     settings->value("db/port").toInt(),
                     settings->value("db/username").toString(),
                     settings->value("db/pw").toString());
    QDateTime begin_ = ui->dateTimeEdit_start->dateTime();
    QDateTime end_ = ui->dateTimeEdit_stop->dateTime();
    worker.fetchPackets(begin_, end_);
}

void MainWindow::eventMode_triggered()
{
    if (!action_EventMode->isChecked()) {
        ui->treeView->setModel(myPacketStore->model);
        settings->setValue("ui/eventmode", false);
    } else {
        ui->treeView->setModel(myEventStore->model);
        settings->setValue("ui/eventmode", true);
    }
}

void MainWindow::loadObjectView(QModelIndex index)
{
    if (myEventStore->itemInStore(index.data().toString())) {
        ObjectView* objView = new ObjectView(this, index, myEventStore->model);
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
