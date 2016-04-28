#ifndef TRANSLATIONVIEWER_H
#define TRANSLATIONVIEWER_H

#include <QDialog>
#include "packetstore.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include "mainwindow.h"

namespace Ui {
class TranslationViewer;
}

class TranslationViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TranslationViewer(QWidget *parent = 0, QHash<QString, QString> *l_objn_ = 0, QHash<QString, QString> *l_evn_ = 0, QHash<QString, QString> *l_pac_ = 0, QHash<int, PI_VALUES> *l_spids_ = 0, QHash<int, PIC_VALUES> *l_types_ = 0);
    ~TranslationViewer();

private:
    Ui::TranslationViewer *ui;

    QStandardItemModel* eventListModel;
    QStandardItemModel* objectListModel;
    QStandardItemModel* SPIDListModel;
    QStandardItemModel* PI_VALUESListModel;
    QStandardItemModel* PIC_VALUESListModel;
    QSortFilterProxyModel* eventListModelProxy;
    QSortFilterProxyModel* objectListModelProxy;
    QSortFilterProxyModel* SPIDListModelProxy;
    QSortFilterProxyModel* PI_VALUESListModelProxy;
    QSortFilterProxyModel* PIC_VALUESListModelProxy;
    QSortFilterProxyModel* selectedProxy;

    MainWindow* pMainWindow;

    QHash<QString,QString>* l_object_names;
    QHash<QString,QString>* l_event_names;
    QHash<QString,QString>* l_packet_names;
    QHash<int,PI_VALUES>* l_spids;
    QHash<int,PIC_VALUES>* l_types;

    void updateInfoText();
    void updateList(QHash<QString, QString>* hash_, QStandardItemModel* model_);
    void updateList(QHash<int, PI_VALUES> *hash_, QStandardItemModel *model_);
    void updateList(QHash<int, PIC_VALUES> *hash_, QStandardItemModel *model_);

private slots:
    void comboBoxSelected(int listIndex_);
    void reload();
    void addTranslation();
    void clearTranslationTable();
    void table_item_right_click(QPoint pos);
    void switch_hex_dec_action();

};

#endif // TRANSLATIONVIEWER_H
