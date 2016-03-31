#ifndef PACKETCONTENTVIEW_H
#define PACKETCONTENTVIEW_H

#include <QDialog>
#include "packetstore.h"

namespace Ui {
class PacketContentView;
}

class PacketContentView : public QDialog
{
    Q_OBJECT

public:
    explicit PacketContentView(QWidget *parent = 0, PacketStore* st_ = 0, int idx_ = 0);
    ~PacketContentView();

private:
    Ui::PacketContentView *ui;
    PacketStore* store;
    SourcePacket* selectedPacket;
};

#endif // PACKETCONTENTVIEW_H
