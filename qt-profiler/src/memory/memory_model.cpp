#include <QFile>
#include <QTextStream>

#include <stdlib.h>

#include "include/memory/memory_model.h"


MemoryModel::MemoryModel() noexcept :
    fileLib_(QString("lib_hook_malloc.so")),
    fileLog_(QString("log")) { }

void MemoryModel::processRequest(const QString& request)
{
    QMap<QString, MallocObject> leaks;

    try {
        this->collectMallocUsage_(request);

        this->readMallocUsage_(leaks);
        if (leaks.size() == 0)
        {
            result_.add(qMakePair(ViewType::source , QString("There are no possible memory leaks")));
            Observable::notify(Event::succses);
            return;
        }

        this->leakToSourceCode_(leaks, request);
    }
    catch (QString& err)
    {
        result_.add(qMakePair(ViewType::error , err));
        Observable::notify(Event::fail);
        return;
    }
    catch (...)
    {
        result_.add(qMakePair(ViewType::error , QString("Unknow error")));
        Observable::notify(Event::fail);
        return;
    }

    Observable::notify(Event::succses);
}

Result MemoryModel::getResult() noexcept
{
    return result_;
}

void MemoryModel::collectMallocUsage_(const QString& elf)
{
    QString arg;

    arg += "LD_PRELOAD=";
    arg += "./";
    arg += fileLib_;
    arg += " ";
    arg += elf;
    arg += " 2>";
    arg += fileLog_;

    if (system(arg.toStdString().c_str()) != 0)
    {
        throw QString("Warning: Can't collect malloc usage for: " + elf);
    }
}

void MemoryModel::readMallocUsage_(QMap<QString, MallocObject>& map)
{
    const QRegExp rx("[ ]");

    QFile file(fileLog_);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
        throw QString("Warning: Can't open file: " + fileLog_);

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
    if (remove(fileLog_.toUtf8().constData()) != 0)
        throw QString("Warning: Can't delete file: " + fileLog_);
}

void MemoryModel::leakToSourceCode_(const QMap<QString, MallocObject>& map, const QString& elf)
{
    const QRegExp rx("[:]");

    Result result;

    for (auto iter : map)
    {
        QString arg;
        arg += ("addr2line -e ");
        arg += elf;
        arg += " ";
        arg += iter.address;
        arg += " > ";
        arg += fileLog_;

        if (system(arg.toStdString().c_str()) != 0)
        {
            throw QString("Warning: Can't process addr2line for: " + elf);
        }

        QFile addr2LineFile(fileLog_);
        if (!addr2LineFile.open(QIODevice::ReadOnly | QFile::Text))
        {
            throw QString("Warning: Can't open file: " + addr2LineFile.errorString());
        }

        QTextStream inTextStream(&addr2LineFile);
        while (!inTextStream.atEnd())
        {
            auto line     = inTextStream.readLine();
            auto argsList = line.split(rx, QString::SkipEmptyParts);

            result.add(qMakePair(ViewType::source, line));

            readSourceCode_(argsList.at(0), argsList.at(1).toInt(), result);
        }

        // Close the file
        addr2LineFile.close();

        // Delete temporal log file
        if (remove(fileLog_.toUtf8().constData()) != 0)
            throw QString("Warning: Can't delete file: " + fileLog_);
    }

    result_ = result;
}

void MemoryModel::readSourceCode_(const QString& path, const int ln, Result& result)
{
    // Get path to source file and open it
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
        throw QString("Warning: Can't open file: " + file.errorString());

    auto cnt = 1;
    QTextStream inTextStream(&file);
    while (!inTextStream.atEnd())
    {
        auto tmp = inTextStream.readLine();

        if ((ln - 3 < cnt) && (ln + 3 > cnt))
        {
            if (ln == cnt)
                result.add(qMakePair(ViewType::leak, QString(QString::number(cnt) + "  " + tmp)));
            else
                result.add(qMakePair(ViewType::line, QString(QString::number(cnt) + "  " + tmp)));
         }

         cnt++;
    }

    // Close the source file
    file.close();
}
