#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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

#endif // DEFINITIONS_H
