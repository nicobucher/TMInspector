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

    void writeSettings();
    void readSettings();

signals:
    void clientSetup(QThread* t_, QString host, int port);

public slots:
    void displayPacketWorkerError(const QString errormessage);
    void displayStatusBarMessage(const QString message);
    void loadObjectView(QModelIndex index);
    void loadObjectView_Arch(QModelIndex index);

private slots:
    void on_actionTo_Server_triggered();
    void closeEvent(QCloseEvent *event);
    void on_actionEdit_triggered();
    void translation_triggered();
    void on_commandLinkButton_clicked();
    void eventMode_triggered();
    void exportTriggered();

private:
    QMenu *dataMenu;
    QAction *action_Connect;
    QAction *action_EventMode;

};

#endif // MAINWINDOW_H
