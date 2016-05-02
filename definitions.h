#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <QCoreApplication>

enum ExtraRoles {
    ReadableTextRole = Qt::UserRole + 1,
    RawDataRole,
    ListIndexRole
};

struct PIC_VALUES {
    int PI1_offset;
    int PI1_width;
    int PI2_offset;
    int PI2_width;
};
Q_DECLARE_METATYPE(PIC_VALUES)

struct PI_VALUES {
    unsigned int PI1_VAL;
    unsigned int PI2_VAL;
    int type_key;
};
Q_DECLARE_METATYPE(PI_VALUES)

#endif // DEFINITIONS_H
