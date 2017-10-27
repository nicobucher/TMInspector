#ifndef SERVICE206_H
#define SERVICE206_H

#include <QObject>
#include "packets/variablepacket.h"

class Service206 : public VariablePacket
{
    Q_OBJECT
public:
    Service206(SourcePacket &packet);

    void decode();

    QVector<QVariant> getValues() const;

    int getParr_obj_id() const;
    void setParr_obj_id(int value);

    int getParr_module_id() const;
    void setParr_module_id(int value);

    int getParr_array_id() const;
    void setParr_array_id(int value);

    int getParr_index() const;
    void setParr_index(int value);

    int getRows() const;
    void setRows(int value);

    int getColumns() const;
    void setColumns(int value);

private:

    int parr_module_id;
    int parr_array_id;
    int parr_index;

    int rows;
    int columns;
};

#endif // SERVICE206_H
