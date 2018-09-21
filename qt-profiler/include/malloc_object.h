#ifndef MALLOC_OBJECT_H
#define MALLOC_OBJECT_H

#include <QString>

enum MallocObjectArg { addr, fun, size, ptr };

class MallocObject
{
    public:
        MallocObject(QString addr, QString fun, QString size, QString ptr);
        ~MallocObject();

        QString _addr;
        QString _fun;
        QString _size;
        QString _ptr;
};

#endif // MALLOC_OBJECT_H
