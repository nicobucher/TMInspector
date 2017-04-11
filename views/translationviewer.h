#ifndef TRANSLATIONVIEWER_H
#define TRANSLATIONVIEWER_H

#include <QDialog>
#include "stores/packetstore.h"
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
    explicit TranslationViewer(QWidget *parent = 0);
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

signals:
    void addObjectWatchlist(QString name);
};

#endif // TRANSLATIONVIEWER_H
