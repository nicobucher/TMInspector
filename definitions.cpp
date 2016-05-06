#include "definitions.h"
#include <QDataStream>

QDataStream &operator<<(QDataStream &out, const PIC_VALUES &myObj)
{
    out << qint32(myObj.PI1_offset) << qint32(myObj.PI1_width) << qint32(myObj.PI2_offset) << qint32(myObj.PI2_width);
    return out;
}

QDataStream &operator>>(QDataStream &in, PIC_VALUES &myObj)
{
    int PI1_offset;
    int PI1_width;
    int PI2_offset;
    int PI2_width;
    in >> PI1_offset >> PI1_width >> PI2_offset >> PI2_width;
    myObj = PIC_VALUES(PI1_offset, PI1_width, PI2_offset, PI2_width);
    return in;
}



QDataStream &operator<<(QDataStream &out, const PI_VALUES &myObj)
{
    out << quint32(myObj.PI1_VAL) << quint32(myObj.PI2_VAL) << qint32(myObj.type_key);
    return out;
}

QDataStream &operator>>(QDataStream &in, PI_VALUES &myObj)
{
    unsigned int PI1_VAL;
    unsigned int PI2_VAL;
    int type_key;
    in >> PI1_VAL >> PI2_VAL >> type_key;
    myObj = PI_VALUES(PI1_VAL, PI2_VAL, type_key);
    return in;
}
