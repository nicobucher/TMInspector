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
    explicit TranslationViewer(QWidget *parent = 0, QHash<QString, QString> *l_objn_ = 0, QHash<QString, QString> *l_evn_ = 0);
    ~TranslationViewer();

private:
    Ui::TranslationViewer *ui;

    QStandardItemModel* eventListModel;
    QStandardItemModel* objectListModel;
    QSortFilterProxyModel* eventListModelProxy;
    QSortFilterProxyModel* objectListModelProxy;
    QSortFilterProxyModel* selectedProxy;

    MainWindow* pMainWindow;

    QHash<QString,QString>* l_object_names;
    QHash<QString,QString>* l_event_names;

    void updateInfoText();
    void updateList(QHash<QString, QString>* hash_, QStandardItemModel* model_);

private slots:
    void comboBoxSelected(int listIndex_);
    void reload();
    void addTranslation();
    void clearTranslationTable();
    void table_item_right_click(QPoint pos);
    void switch_hex_dec_action();

};

#endif // TRANSLATIONVIEWER_H
