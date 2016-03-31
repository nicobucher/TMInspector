#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QSettings>
#include <QThread>
#include "serversettings.h"
#include "packetworker.h"
#include "packetstore.h"
#include "eventstore.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void addTranslation(QString key_, QString trans_, int list_index_);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PacketWorker* myPacketWorker;
    QThread* myPacketWorkerThread;

    PacketStore* myPacketStore;
    PacketStore* mySqlPacketStore;
    EventStore* myEventStore;
    EventStore* mySqlEventStore;

    QLabel* statusLabel;

    // Permanent Settings
    QSettings* settings;

    // Popup Dialogue Windows
    ServerSettings* serverSettingsWindow;

    QHash<QString,QString> l_event_names;
    QHash<QString,QString> l_object_names;

    bool treeviewExpanded;
    bool treeviewExpanded_Arch;

    void writeSettings();
    void readSettings();

    void populateEventHash(QSqlDatabase* db_);
    void populateObjectHash(QSqlDatabase* db_);

signals:
    void clientSetup(QThread* t_, QString host, int port);
    void hashUpdated();

public slots:
    void displayPacketWorkerError(const QString errormessage);
    void displayStatusBarMessage(const QString message);
    void loadObjectView(QModelIndex index);
    void loadTranslationTable();
    void animateNewEvent(Event* event);

private slots:
    void on_actionTo_Server_triggered();
    void closeEvent(QCloseEvent *event);
    void on_actionEdit_triggered();
    void translation_triggered();
    void on_commandLinkButton_clicked();
    void eventMode_triggered();
    void exportTriggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    QMenu *dataMenu;
    QAction *action_Connect;
    QAction *action_EventMode;

};

#endif // MAINWINDOW_H
