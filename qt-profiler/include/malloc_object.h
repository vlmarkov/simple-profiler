#pragma once

#include <QString>


enum MallocObjectArg { addr, fun, size, ptr };

class MallocObject
{
    public:
        MallocObject(QString addr, QString fun, QString sz, QString ptr) noexcept;
        ~MallocObject() = default;

        QString address;
        QString function;
        QString size;
        QString pointer;
};
