#ifndef VARIABLEPACKET_H
#define VARIABLEPACKET_H

#include <QObject>
#include "packets/sourcepacket.h"

class VariablePacket : public SourcePacket
{
    Q_OBJECT
public:
    VariablePacket(SourcePacket &packet);

    virtual void decode();

    int getPfc() const;
    void setPfc(int value);

    int getPtc() const;
    void setPtc(int value);

    QVector<QVariant> getValues() const;

    int getParr_obj_id() const;
    void setParr_obj_id(int value);

protected:
    int pfc;
    int ptc;

    int parr_obj_id;

    QVector<QVariant> values;
};

#endif // VARIABLEPACKET_H
