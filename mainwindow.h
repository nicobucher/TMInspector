#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QSettings>
#include <QThread>
#include <QProgressDialog>
#include "serversettings.h"
#include "packetworker.h"
#include "packetstore.h"
#include "eventstore.h"
#include "custommodels.h"
#include "definitions.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include "translator.h"

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

    void addObjectToWatchList(QString object_name_);

private:
    Ui::MainWindow *ui;
    PacketWorker* myPacketWorker;
    QThread* myPacketWorkerThread;
    QThread* mySqlWorkerThread;

    PacketStore* myPacketStore;
    PacketStore* mySqlPacketStore;
    EventStore* myEventStore;
    EventStore* mySqlEventStore;

    QLabel* statusLabel;

    // Filter Elements
    QHBoxLayout* SqlEventFilterLayout;
    QHBoxLayout* LiveEventFilterLayout;

    QHBoxLayout* LivePacketFilterLayout;
    QHBoxLayout* SqlPacketFilterLayout;

    QLineEdit* SqlRegFilter;
    QLineEdit* LiveRegFilter;

    QLineEdit* SqlTypeFilter;
    QLineEdit* LiveTypeFilter;

    QPushButton* LiveExpandAll;
    QPushButton* SqlExpandAll;

    QString currentSqlRegEx;
    QString currentSqlType;
    QString currentLiveRegEx;
    QString currentLiveType;

private slots:
    void set_currentSqlRegEx(QString txt_){
        currentSqlRegEx = txt_;
    }
    void set_currentSqlType(QString txt_){
        currentSqlType = txt_;
    }
    void set_currentLiveRegEx(QString txt_){
        currentLiveRegEx = txt_;
    }
    void set_currentLiveType(QString txt_){
        currentLiveType = txt_;
    }

private:

    // Permanent Settings
    QSettings* settings;

    // Popup Dialogue Windows
    ServerSettings* serverSettingsWindow;

    // Object Translators
    SPIDTranslator* mySPIDTranslator;
    PICTranslator* myPICTranslator;
    PITranslator* myPITranslator;
    ObjectTranslator* myObjectTranslator;
    EventTranslator* myEventTranslator;


//    QHash<QString,QString> l_event_names;
//    QHash<QString,QString> l_object_names;
//    QHash<QString,QString> l_packet_names;
//    QHash<int,PI_VALUES> l_spids;
//    QHash<int,PIC_VALUES> l_types;

    // Object WatchList Model
    StringList* watch_list_model;

    bool treeviewExpanded;
    bool treeviewExpanded_Arch;

    void writeSettings();
    void readSettings();

//    void populateEventHash(QSqlDatabase* db_);
//    void populateObjectHash(QSqlDatabase* db_);
//    void populatePacketHash(QSqlDatabase *db_);
//    void populateSPIDHash(QSqlDatabase *db_);
//    void populateTypesHash(QSqlDatabase *db_);

    QProgressDialog* progress_;

signals:
    void clientSetup(QThread* t_, QString host, int port);
    void hashUpdated();

public slots:
    void displayPacketWorkerError(const QString errormessage);
    void sqlWorkerFinished();
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
    void tree_item_right_click(QPoint p_);
    void show_packet_action();

    void live_expand_all_clicked();
    void sql_expand_all_clicked();

    void addToWatchlist_clicked();

private:
    QMenu *dataMenu;
    QAction *action_Connect;
    QAction *action_EventMode;

    void setupEventFilters();
    void setupPacketFilters();
    void removeAndeDeleteLayout(QLayout *layout_);
};

#endif // MAINWINDOW_H
