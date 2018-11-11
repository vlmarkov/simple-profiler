#pragma once

#include <QString>


enum MemLeakObjArg { addr, fun, size, ptr };


class MemLeakObj
{
    public:
        MemLeakObj(QString addr, QString fun, QString sz, QString ptr) noexcept;
        ~MemLeakObj() = default;

        QString address;
        QString function;
        QString size;
        QString pointer;
};
