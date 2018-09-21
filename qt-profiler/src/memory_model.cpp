#include <QFile>
#include <QTextStream>

#include <stdlib.h>

#include "include/memory_model.h"


MemoryModel::MemoryModel()
{
    libFileName_ = "lib_hook_malloc.so";
    logFileName_ = "log";
}

MemoryModel::~MemoryModel()
{
    ;
}

void MemoryModel::processRequest(const QString& str)
{
    QMap<QString, MallocObject> leakMap;

    try {
        this->collectMallocUsage_(str);

        this->readMallocUsage_(leakMap);
        if (leakMap.size() == 0)
        {
            this->result_.addData(qMakePair(ViewType::source , QString("There are no possible memory leaks")));
            this->notifyObserver(Event::succses);
            return;
        }

        this->leakToSourceCode_(leakMap, str);
    }
    catch (QString& err)
    {
        result_.addData(qMakePair(ViewType::error , err));
        this->notifyObserver(Event::fail);
        return;
    }

    this->notifyObserver(Event::succses);
}

Result MemoryModel::getResult()
{
    return result_;
}

void MemoryModel::collectMallocUsage_(const QString& elf)
{
    QString arg;

    arg += "LD_PRELOAD=";
    arg += "./";
    arg += this->libFileName_;
    arg += " ";
    arg += elf;
    arg += " 2>";
    arg += this->logFileName_;

    if (system(arg.toStdString().c_str()) != 0)
    {
        throw QString("Warning: Can't collect malloc usage for: " + elf);
    }
}

void MemoryModel::readMallocUsage_(QMap<QString, MallocObject>& map)
{
    const QRegExp rx("[ ]");

    QFile file(this->logFileName_);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        throw QString("Warning: Can't open file: " + this->logFileName_);
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        // Read line and split it
        auto argsList = in.readLine().split(rx, QString::SkipEmptyParts);

        if (argsList.at(MallocObjectArg::fun) == "free")
        {
            auto key = argsList.at(MallocObjectArg::ptr);
            map.remove(key);
        }

        if (argsList.at(MallocObjectArg::fun) == "malloc" || argsList.at(MallocObjectArg::fun) == "calloc")
        {
            auto key = argsList.at(MallocObjectArg::ptr);
            auto value = MallocObject(argsList.at(MallocObjectArg::addr),
                                      argsList.at(MallocObjectArg::fun),
                                      argsList.at(MallocObjectArg::size),
                                      argsList.at(MallocObjectArg::ptr));

            map.insert(key, value);
        }

        if (argsList.at(MallocObjectArg::fun) == "realloc")
        {
            auto key = argsList.at(MallocObjectArg::ptr);
            auto value = MallocObject(argsList.at(MallocObjectArg::addr),
                                      argsList.at(MallocObjectArg::fun),
                                      argsList.at(MallocObjectArg::size),
                                      argsList.at(MallocObjectArg::ptr));

            map.remove(key);
            map.insert(key, value);
        }
    }

    // Close the file
    file.close();

    // Delete temporal log file
    if (remove(this->logFileName_.toUtf8().constData()) != 0)
    {
        throw QString("Warning: Can't delete file: " + this->logFileName_);
    }
}

void MemoryModel::leakToSourceCode_(const QMap<QString, MallocObject>& map, const QString& elf)
{
    const QRegExp rx("[:]");

    Result res;

    for (auto it : map)
    {
        QString arg;
        arg += ("addr2line -e ");
        arg += elf;
        arg += " ";
        arg += it._addr;
        arg += " > ";
        arg += this->logFileName_;

        if (system(arg.toStdString().c_str()) != 0)
        {
            throw QString("Warning: Can't process addr2line for: " + elf);
        }

        QFile addr2LineFile(this->logFileName_);
        if (!addr2LineFile.open(QIODevice::ReadOnly | QFile::Text))
        {
            throw QString("Warning: Can't open file: " + addr2LineFile.errorString());
        }

        QTextStream in(&addr2LineFile);
        while (!in.atEnd())
        {
            auto line     = in.readLine();
            auto argsList = line.split(rx, QString::SkipEmptyParts);

            res.addData(qMakePair(ViewType::source, line));

            this->readSourceCode_(argsList.at(0), argsList.at(1).toInt(), res);
        }

        // Close the file
        addr2LineFile.close();

        // Delete temporal log file
        if (remove(this->logFileName_.toUtf8().constData()) != 0)
        {
            throw QString("Warning: Can't delete file: " + this->logFileName_);
        }
    }

    this->result_ = res;
}

void MemoryModel::readSourceCode_(const QString& path, const int ln, Result& res)
{
    // Get path to source file and open it
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        throw QString("Warning: Can't open file: " + file.errorString());
    }

    int cnt = 1;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        auto str = in.readLine();

        if ((ln - 3 < cnt) && (ln + 3 > cnt))
        {
            if (ln == cnt)
            {
                res.addData(qMakePair(ViewType::leak, QString(QString::number(cnt) + "  " + str)));
            }
            else
            {
                res.addData(qMakePair(ViewType::line, QString(QString::number(cnt) + "  " + str)));
            }
         }
         cnt++;
    }

    // Close the source file
    file.close();
}
