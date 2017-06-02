#ifndef VARIANTPOINTER_H
#define VARIANTPOINTER_H

template <class T> class VariantPtr
{
public:
    static T* asPtr(QVariant v)
    {
        return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr)
    {
        return qVariantFromValue((void *) ptr);
    }
};

#endif // VARIANTPOINTER_H
