#ifndef TRANSLATIONVIEWER_H
#define TRANSLATIONVIEWER_H

#include <QDialog>
#include "packetstore.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include "mainwindow.h"
#include "translator.h"

namespace Ui {
class TranslationViewer;
}

class TranslationViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TranslationViewer(QWidget *parent = 0,
                               ObjectTranslator* obj_trans_ = 0,
                               EventTranslator* event_trans_ = 0,
                               SPIDTranslator* spid_trans_ = 0,
                               PITranslator* pi_trans_ = 0,
                               PICTranslator* pic_trans_ = 0);
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

    ObjectTranslator* myObjectTranslator;
    EventTranslator* myEventTranslator;
    SPIDTranslator* mySPIDTranslator;
    PITranslator* myPITranslator;
    PICTranslator* myPICTranslator;

    void updateInfoText();
    void updateList(QHash<int, QVariant>* hash_, QStandardItemModel* model_);
    void updatePIList(QHash<int, QVariant> *hash_, QStandardItemModel *model_);
    void updatePICList(QHash<int, QVariant> *hash_, QStandardItemModel *model_);

private slots:
    void comboBoxSelected(int listIndex_);
    void reload();
    void addTranslation();
    void clearTranslationTable();
    void table_item_right_click(QPoint pos);
    void switch_hex_dec_action();

};

#endif // TRANSLATIONVIEWER_H
