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

    void updateInfoText();

private slots:
    void comboBoxSelected(int listIndex_);
};

#endif // TRANSLATIONVIEWER_H
