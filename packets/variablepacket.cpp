#include "variablepacket.h"

VariablePacket::VariablePacket(SourcePacket &packet) : SourcePacket(packet)
{
    decode();
}

void VariablePacket::decode()
{
    unsigned char* p_ = &this->data[12];
    this->parr_obj_id = (p_[0] << 24) + (p_[1] << 16) + (p_[2] << 8) + p_[3];
    // Extract the dump id and dump counter
    this->parr_module_id = p_[4];
    this->parr_array_id = (p_[5] << 8) + p_[6];
    this->parr_index = p_[7];

    this->setPtc( p_[8] );
    this->setPfc( p_[9] );

    this->setColumns( p_[10] );
    this->setRows( p_[11] );

    int datalength = 1;
    switch(ptc) {
    case 3:
        if (pfc == 12) {
            datalength = 2;
        } else if (pfc == 14) {
            datalength = 4;
        } else if (pfc == 16) {
            datalength = 8;
        }
        break;
    case 4:
        if (pfc == 12) {
            datalength = 2;
        } else if (pfc == 14) {
            datalength = 4;
        } else if (pfc == 16) {
            datalength = 8;
        }
        break;
    case 5:
        if (pfc == 1 || pfc == 3) {
            datalength = 4;
        } else if (pfc == 2) {
            datalength = 8;
        }
        break;
    case 8:
        if (pfc == 0) {
            // TODO length??
        } else {
            datalength = pfc;
        }
        break;
    }

    int data_size = this->columns * this->rows * datalength;

    p_ = p_ + 11;
    int i = 0;
    while (i < data_size) {
        switch(ptc) {
        case 1:
            if (pfc == 0) {
                this->values.append(QVariant((bool)p_[i]));
            }
            break;
        case 3:
            if (pfc == 4) {
                this->values.append(QVariant((quint8)p_[i]));
            } else if (pfc == 12) {
                this->values.append(QVariant((quint16)((p_[i] << 8) + p_[i + 1])));
            } else if (pfc == 14) {
                this->values.append(QVariant((quint32)((p_[i] << 24) + (p_[i + 1] << 16) + (p_[i + 2] << 8) + p_[i + 3])));
            } else if (pfc == 16) {
                this->values.append(QVariant((quint64)((p_[i] << 56) + (p_[i + 1] << 48) + (p_[i + 2] << 40) + (p_[i + 3] << 32) + (p_[i + 4] << 24) + (p_[i + 5] << 16) + (p_[i + 6] << 8) + p_[i + 7])));
            }
            break;
        case 4:
            if (pfc == 4) {
                this->values.append(QVariant((qint8)p_[i]));
            } else if (pfc == 12) {
                this->values.append(QVariant((qint16)((p_[i] << 8) + p_[i + 1])));
            } else if (pfc == 14) {
                this->values.append(QVariant((qint32)((p_[i] << 24) + (p_[i + 1] << 16) + (p_[i + 2] << 8) + p_[i + 3])));
            } else if (pfc == 16) {
                this->values.append(QVariant((qint64)((p_[i] << 56) + (p_[i + 1] << 48) + (p_[i + 2] << 40) + (p_[i + 3] << 32) + (p_[i + 4] << 24) + (p_[i + 5] << 16) + (p_[i + 6] << 8) + p_[i + 7])));
            }
            break;
        case 5:
            if (pfc == 1 || pfc == 3) {
                this->values.append(QVariant((float)((p_[i] << 24) + (p_[i + 1] << 16) + (p_[i + 2] << 8) + p_[i + 3])));
            } else if (pfc == 2) {
                this->values.append(QVariant((double)((p_[i] << 56) + (p_[i + 1] << 48) + (p_[i + 2] << 40) + (p_[i + 3] << 32) + (p_[i + 4] << 24) + (p_[i + 5] << 16) + (p_[i + 6] << 8) + p_[i + 7])));
            }
            break;
        case 8:
            if (pfc == 0) {
                // TODO boundary checks...
                // TODO length??
            } else {
                // TODO boundary checks...
                this->values.append(QString((QChar*)p_, pfc));
            }
            break;
        }
        i = i + datalength;
    }
}

int VariablePacket::getPfc() const
{
    return pfc;
}

void VariablePacket::setPfc(int value)
{
    pfc = value;
}

int VariablePacket::getPtc() const
{
    return ptc;
}

void VariablePacket::setPtc(int value)
{
    ptc = value;
}

QVector<QVariant> VariablePacket::getValues() const
{
    return values;
}

int VariablePacket::getParr_obj_id() const
{
    return parr_obj_id;
}

void VariablePacket::setParr_obj_id(int value)
{
    parr_obj_id = value;
}

int VariablePacket::getParr_module_id() const
{
    return parr_module_id;
}

void VariablePacket::setParr_module_id(int value)
{
    parr_module_id = value;
}

int VariablePacket::getParr_array_id() const
{
    return parr_array_id;
}

void VariablePacket::setParr_array_id(int value)
{
    parr_array_id = value;
}

int VariablePacket::getParr_index() const
{
    return parr_index;
}

void VariablePacket::setParr_index(int value)
{
    parr_index = value;
}

int VariablePacket::getRows() const
{
    return rows;
}

void VariablePacket::setRows(int value)
{
    rows = value;
}

int VariablePacket::getColumns() const
{
    return columns;
}

void VariablePacket::setColumns(int value)
{
    columns = value;
}
