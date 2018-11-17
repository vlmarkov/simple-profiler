#pragma once

#include <QMap>
#include <QFile>
#include <QString>

#include <include/profiler/result.hpp>


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

class FileReader
{
    protected:
        QFile file_;

    public:
        explicit FileReader(const QString& fileName);
        ~FileReader();
};

class FileReaderMemLeak : public FileReader
{
    public:
        explicit FileReaderMemLeak(const QString& fileName);
        QMap<QString, MemLeakObj> readMemLeaks();
};

class FileReaderSource : public FileReader
{
    public:
        explicit FileReaderSource(const QString& fileName);
        void readSource(Result& result);
};

class FileReaderSourceLine : public FileReader
{
    public:
        explicit FileReaderSourceLine(const QString& fileName);
        void readSourceLine(const int ln, Result& result);
};
