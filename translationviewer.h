#ifndef TRANSLATIONVIEWER_H
#define TRANSLATIONVIEWER_H

#include <QDialog>
#include "eventstore.h"
#include "packetstore.h"
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class TranslationViewer;
}

class TranslationViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TranslationViewer(QWidget *parent = 0, EventStore *event_store_ = 0);
    ~TranslationViewer();

private:
    Ui::TranslationViewer *ui;

    QStandardItemModel* eventListModel;
    QStandardItemModel* objectListModel;
    QStandardItemModel* selectedModel;

    EventStore* event_store;

    void updateInfoText();
    void updateList(QHash<QString, QString>* hash_, QStandardItemModel* model_);

private slots:
    void comboBoxSelected(int listIndex_);
    void reload();

};

#endif // TRANSLATIONVIEWER_H
