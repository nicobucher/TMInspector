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

private:
    Ui::Checksumview *ui;
    QStandardItemModel *model;

public slots:
    void receiveChecksum(uint32_t address, uint16_t checksum);

private slots:
    void loadChecksumFile();
    void clear();
};

#endif // CHECKSUMVIEW_H
