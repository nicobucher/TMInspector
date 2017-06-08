#ifndef VARIABLEPACKET_H
#define VARIABLEPACKET_H

#include <QObject>
#include "packets/sourcepacket.h"

class VariablePacket : public SourcePacket
{
    Q_OBJECT
public:
    VariablePacket(SourcePacket &packet);

    void decode();

    int getPfc() const;
    void setPfc(int value);

    int getPtc() const;
    void setPtc(int value);

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
    int pfc;
    int ptc;

    int parr_obj_id;
    int parr_module_id;
    int parr_array_id;
    int parr_index;

    int rows;
    int columns;

    QVector<QVariant> values;
};

#endif // VARIABLEPACKET_H
