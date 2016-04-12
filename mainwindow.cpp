#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objectview.h"
#include "translationviewer.h"
#include "packetcontentview.h"
#include "sqlworker.h"
#include "filehelpers.h"
#include <QTableView>
#include <QDateTime>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("Institute of Space Systems");
    QCoreApplication::setOrganizationDomain("www.irs.uni-stuttgart.de");
    QCoreApplication::setApplicationName("TMInspector");

    ui->setupUi(this);

    settings = new QSettings();

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
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::green);
    statusLabel->setPalette(pal);
    statusLabel->setText("Disconneted");
    this->statusBar()->addPermanentWidget(statusLabel);

    myPacketWorker = 0;
    myPacketWorkerThread = 0;

    myPacketStore = new PacketStore(this);
    mySqlPacketStore = new PacketStore(this);
    myEventStore = new EventStore(this, settings, &l_object_names, &l_event_names);
    mySqlEventStore = new EventStore(this, settings, &l_object_names, &l_event_names);

    dataMenu->addAction("Translation Table", this, SLOT(translation_triggered()));

    // Read the global settings
    readSettings();

    // Setup the treeviews
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

    // Double Click actions
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadObjectView(QModelIndex)));
    connect(ui->treeView_arch, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadObjectView(QModelIndex)));

    // Right Click menu
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView_arch->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tree_item_right_click(QPoint)));
    connect(ui->treeView_arch, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tree_item_right_click(QPoint)));


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
    testevent.setPacketReference(1);
    myEventStore->putEvent(&testevent);

    Event testevent1(now, (Severity)2);
    testevent1.setEventId(2);
    testevent1.setObjectId(1000);
    testevent1.setObjectName("Bla");
    testevent1.setParams(23, 24);
    testevent1.setPacketReference(1);
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

    this->l_event_names = FileHelpers::loadHash("event_names.dat");
    this->l_object_names = FileHelpers::loadHash("object_names.dat");

    // --> Todo: Test Code (remove)
    l_object_names.insert("Test-Event","Test-Object");
    l_event_names.insert("1","Test-Event");
    // <--

    treeviewExpanded = false;
    treeviewExpanded_Arch = false;

    watch_list_model = new StringList();
    ui->listView->setModel(watch_list_model);
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
        statusLabel->setStyleSheet("");
        action_Connect->setText("Connect");
        return;
    }

    myPacketWorker = new PacketWorker(myPacketStore, myEventStore);
    connect(myPacketWorker, SIGNAL(hasError(const QString&)), this, SLOT(displayPacketWorkerError(const QString&)));
    connect(myPacketWorker, SIGNAL(eventAdded(Event*)), this, SLOT(animateNewEvent(Event*)));
    connect(this, SIGNAL(clientSetup(QThread*,QString,int)), myPacketWorker, SLOT(setup(QThread*,QString,int)));
    myPacketWorkerThread = new QThread();

    emit clientSetup(myPacketWorkerThread, settings->value("server/host").toString(), settings->value("server/port").toInt());
    if (myPacketWorkerThread != 0 && myPacketWorker != 0) {
        if (myPacketWorker->isReady) {
            myPacketWorker->moveToThread(myPacketWorkerThread);
            myPacketWorkerThread->start();
            statusLabel->setText("Connected");
            statusLabel->setStyleSheet("background-color:#00CC00;");
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
    statusLabel->setStyleSheet("");
    statusLabel->setText(tr("Disconnected"));
    action_Connect->setText("Connect");
}

void MainWindow::on_commandLinkButton_clicked()
{
    ui->commandLinkButton->setEnabled(false);

    mySqlPacketStore->emptyStore();
    mySqlEventStore->emptyStore();

    progress_ = new QProgressDialog("Loading Packets from Database","Cancel",0,100);
    progress_->setMinimumDuration(0);
    progress_->reset();
    progress_->show();

    QDateTime begin_ = ui->dateTimeEdit_start->dateTime();
    QDateTime end_ = ui->dateTimeEdit_stop->dateTime();
    SqlWorker* worker = new SqlWorker(settings, begin_, end_, mySqlPacketStore, mySqlEventStore, progress_);
    connect(worker, SIGNAL(dbAccessError(QString)), this, SLOT(displayStatusBarMessage(QString)));
    connect(worker, SIGNAL(progressMade(int)), progress_, SLOT(setValue(int)));
    connect(worker, SIGNAL(newMaxProgress(int)), progress_, SLOT(setMaximum(int)));
    connect(worker, SIGNAL(newText(QString)), progress_, SLOT(setLabelText(QString)));
    connect(worker, SIGNAL(finished()), this, SLOT(sqlWorkerFinished()));

    mySqlWorkerThread = new QThread();

    worker->moveToThread(mySqlWorkerThread);
    connect(mySqlWorkerThread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(progress_, SIGNAL(canceled()), worker, SLOT(abortWork()));
    mySqlWorkerThread->start();

    ui->commandLinkButton->setEnabled(true);
}

void MainWindow::sqlWorkerFinished()
{
    progress_->hide();
    mySqlWorkerThread->quit();
    mySqlWorkerThread->wait();
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
    Store* selectedStore;
    if (index.model() == myEventStore->proxy_model || index.model() == mySqlEventStore->proxy_model) {
        selectedStore = (Store*)index.model()->parent();
        if (selectedStore->itemInStore(index.data().toString())) {
            // The mapping to the source model is required because index is of the proxy_model and needs to be mapped to the source model in order to be resolved
            QModelIndex sourceIndex = selectedStore->proxy_model->mapToSource(index);
            // Then pass the mapped sourceIndex to the ObjectView
            ObjectView* objView = new ObjectView(this, sourceIndex, selectedStore->getModel());
            objView->setAttribute(Qt::WA_DeleteOnClose);
            objView->show();
            objView->raise();
            objView->activateWindow();
        }
        return;
    }
    if (index.model() == myPacketStore->proxy_model || index.model() == mySqlPacketStore->proxy_model) {
        selectedStore = (Store*)index.model()->parent();
        // Get the index from the item in column zero... This can then be used to look up the packet in the stores packet-list
        QModelIndex pktIndex = index.model()->index(index.row(),0);
        int pkt_id = pktIndex.data(ListIndexRole).toInt();
        PacketContentView* pktView = new PacketContentView(this, (PacketStore*)selectedStore, pkt_id);
        pktView->setAttribute(Qt::WA_DeleteOnClose);
        pktView->show();
        pktView->raise();
        pktView->activateWindow();
    }
}

void MainWindow::translation_triggered()
{
    TranslationViewer* transView = new TranslationViewer(this, &l_object_names, &l_event_names);
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
    if (ui->tabWidget->currentIndex() == 0) { // Archive Tab
        if (!action_EventMode->isChecked()) {
            mySqlPacketStore->exportToFile(filename);
        } else {
            mySqlEventStore->exportToFile(filename);
        }
    } else if (ui->tabWidget->currentIndex() == 1) { // Live Tab
        if (!action_EventMode->isChecked()) {
            myPacketStore->exportToFile(filename);
        } else {
            myEventStore->exportToFile(filename);
        }
    }
}

void MainWindow::tree_item_right_click(QPoint p_)
{
    QModelIndexList l_indexes;
    if(ui->tabWidget->currentIndex() == 0) {
        l_indexes=ui->treeView_arch->selectionModel()->selectedIndexes();
    } else if (ui->tabWidget->currentIndex() == 1) {
        l_indexes=ui->treeView->selectionModel()->selectedIndexes();
    }
    if (l_indexes.count() > 0) {
        QMenu* menu=new QMenu(this);
        Store* selectedStore = (Store*)l_indexes.at(0).model()->parent();
        // Check if the user clicked on an object or an event (check if the item is in the store)
        if (selectedStore->itemInStore(l_indexes.at(0).data().toString())) {
            QAction* add_watchlist = new QAction("Add to Watch List", this);
            add_watchlist->setData(l_indexes.at(0));
            menu->addAction(add_watchlist);

            connect(add_watchlist, SIGNAL(triggered()), this, SLOT(addToWatchlist_clicked()));
        } else {
            // Item not found in store -> means we have not clicked on an object
            QAction* packet_inspect = new QAction("Show Packet", this);
            packet_inspect->setData(l_indexes.at(0));
            menu->addAction(packet_inspect);

            connect(packet_inspect, SIGNAL(triggered()), this, SLOT(show_packet_action()));
        }

        menu->popup(ui->treeView->viewport()->mapToGlobal(p_));
    }
}

void MainWindow::addToWatchlist_clicked()
{
    // This is used to determine the item that was clicked...
    QAction* pAction = qobject_cast<QAction*>(sender());
    QModelIndex clicked_item_index = pAction->data().toModelIndex();

    addObjectToWatchList(clicked_item_index.data(Qt::DisplayRole).toString());
}

void MainWindow::addObjectToWatchList(const QString object_name_)
{
    *watch_list_model << object_name_;
}

void MainWindow::show_packet_action()
{
    Store* selectedStore;
    if(ui->tabWidget->currentIndex() == 0) {
        selectedStore = mySqlPacketStore;
    } else if (ui->tabWidget->currentIndex() == 1) {
        selectedStore = myPacketStore;
    } else {
        return;
    }

    // This is used to determine the item that was clicked...
    QAction* pAction = qobject_cast<QAction*>(sender());
    QModelIndex clicked_item_index = pAction->data().toModelIndex();

    int pkt_id = clicked_item_index.data(ListIndexRole).toInt();
    PacketContentView* pktView = new PacketContentView(this, (PacketStore*)selectedStore, pkt_id);
    pktView->setAttribute(Qt::WA_DeleteOnClose);
    pktView->show();
    pktView->raise();
    pktView->activateWindow();
}

void MainWindow::loadTranslationTable()
{
    // Get the translation data from the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(settings->value("db/host").toString());
    db.setPort(settings->value("db/port").toInt());
    db.setDatabaseName(settings->value("mib/db").toString());
    db.setUserName(settings->value("mib/user").toString());
    db.setPassword(settings->value("mib/pw").toString());

    if (db.open()) {
        populateEventHash(&db);
        populateObjectHash(&db);
        db.close();
    } else {
        qDebug() << "SQL Error";
        return;
    }

    // Save the hash tables for reuse upon application start
    if (!FileHelpers::saveHash("event_names.dat", l_event_names)) {
        qWarning() << "Can not save hash in event_names.dat";
    }
    if (!FileHelpers::saveHash("object_names.dat", l_object_names)) {
        qWarning() << "Can not save hash in object_names.dat";
    }
    emit hashUpdated();
}

void MainWindow::populateEventHash(QSqlDatabase* db_)
{
    QString str;
    QTextStream(&str) << "SELECT Failure_Event_ID, Failure_Event_Name FROM obsw_events;";
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        l_event_names.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            l_event_names.insert(rec.value(0).toString(), rec.value(1).toString());
        }
    }
}

void MainWindow::populateObjectHash(QSqlDatabase* db_)
{
    QString str;
    QTextStream(&str) << "SELECT TXP_FROM, TXP_ALTXT FROM txp WHERE TXP_NUMBR = 'YMX00005';"; // <- All Object-IDs have the calibration id 'YMX00005'
    QSqlQuery query(str, *db_);
    if (query.size() > 0) {
        l_object_names.clear();
        while (query.next()) {
            QSqlRecord rec = query.record();
            l_object_names.insert(rec.value(0).toString(), rec.value(1).toString());
        }
    }
}

void MainWindow::addTranslation(QString key_, QString trans_, int list_index_)
{
    switch(list_index_) {
    case EventListIndex:
        l_event_names.insert(key_, trans_);
        break;
    case ObjectListIndex:
        l_object_names.insert(key_, trans_);
        break;
    }
    emit hashUpdated();
}

void MainWindow::animateNewEvent(Event* event)
{
    QDateTime now = QDateTime::currentDateTime();
    if (event->getTimestamp().secsTo(now) < 60*60) { // If the event was generated within the last hour
        event->getSeverityItem()->animate();
    }
}

void MainWindow::on_pushButton_clicked()
{
    treeviewExpanded = !treeviewExpanded;
    QModelIndex index;
    for (int row = 0; row < myEventStore->getNumberOfItems(); ++row) {
        index = myEventStore->proxy_model->index(row, 0);
        ui->treeView->setExpanded(index, treeviewExpanded);
    }
    if (treeviewExpanded) {
        ui->pushButton->setText("CloseAll");
    } else {
        ui->pushButton->setText("ExpandAll");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    treeviewExpanded_Arch = !treeviewExpanded_Arch;
    QModelIndex index;
    for (int row = 0; row < mySqlEventStore->getNumberOfItems(); ++row) {
        index = mySqlEventStore->proxy_model->index(row, 0);
        ui->treeView_arch->setExpanded(index, treeviewExpanded_Arch);
    }
    if (treeviewExpanded_Arch) {
        ui->pushButton_2->setText("CloseAll");
    } else {
        ui->pushButton_2->setText("ExpandAll");
    }
}
