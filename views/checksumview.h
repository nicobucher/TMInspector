#ifndef CHECKSUMVIEW_H
#define CHECKSUMVIEW_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class Checksumview;
}

class ChecksumView : public QDialog
{
    Q_OBJECT

public:
    explicit ChecksumView(QWidget *parent = 0);
    ~ChecksumView();

    bool getAutoFocus();

private:
    Ui::Checksumview *ui;
    QStandardItemModel *model;

public slots:
    void receiveChecksum(qint32 address, qint16 checksum);

private slots:
    void loadChecksumFile();
    void clear();
};

#endif // CHECKSUMVIEW_H
