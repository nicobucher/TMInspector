#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <QCoreApplication>

enum ExtraRoles {
    ReadableTextRole = Qt::UserRole + 1,
    RawDataRole,
    ListIndexRole,
    IdentifierRole,
    StorePointerRole,
    TimestampRole,
    ObjectRole
};

struct PIC_VALUES {
    int PI1_offset;
    int PI1_width;
    int PI2_offset;
    int PI2_width;
    PIC_VALUES() :
        PI1_offset(0), PI1_width(0), PI2_offset(0), PI2_width(0) {}
    PIC_VALUES(int PI1_offset_, int PI1_width_, int PI2_offset_, int PI2_width_) :
        PI1_offset(PI1_offset_), PI1_width(PI1_width_), PI2_offset(PI2_offset_), PI2_width(PI2_width_) {}
};
Q_DECLARE_METATYPE(PIC_VALUES)

QDataStream &operator<<(QDataStream &out, const PIC_VALUES &myObj);
QDataStream &operator>>(QDataStream &in, PIC_VALUES &myObj);


struct PI_VALUES {
    unsigned int PI1_VAL;
    unsigned int PI2_VAL;
    int type_key;
    PI_VALUES() :
        PI1_VAL(0), PI2_VAL(0), type_key(0) {}
    PI_VALUES(unsigned int PI1_VAL_, unsigned int PI2_VAL_, int type_key_) :
        PI1_VAL(PI1_VAL_), PI2_VAL(PI2_VAL_), type_key(type_key_) {}
};
Q_DECLARE_METATYPE(PI_VALUES)

QDataStream &operator<<(QDataStream &out, const PI_VALUES &myObj);
QDataStream &operator>>(QDataStream &in, PI_VALUES &myObj);

#endif // DEFINITIONS_H
