#include <QFile>
#include <QTextStream>

#include <stdlib.h>

#include <include/profiler/memory/memory-model.hpp>


MemoryModel::MemoryModel() noexcept :
    fileLib_(QString("lib_hook_malloc.so")),
    fileLog_(QString("log")) { }

void MemoryModel::requestProcess(const QString& request)
{
    QMap<QString, MallocObject> leaks;

    auto event = Event::fail;

    try {
        run_(request);

        read_(leaks);

        leakToSourceCode_(leaks, request);

        event = Event::succses;
    }
    catch (QString& exception)
    {
        result_.add(qMakePair(IViewType::error , exception));
    }
    catch (...)
    {
        result_.add(qMakePair(IViewType::error , QString("Unknow error")));
    }

    Observable::notify(event);
}

Result MemoryModel::getResult() noexcept
{
    return result_;
}

void MemoryModel::run_(const QString& request)
{
    QString cmd("LD_PRELOAD=./" + fileLib_ + " " + request + " 2>" + fileLog_);

    if (system(cmd.toStdString().c_str()) != 0)
        throw QString("Warning: Can't run: " + request);
}

void MemoryModel::read_(QMap<QString, MallocObject>& map)
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

        if ((argsList.at(MallocObjectArg::fun) == "malloc") ||
            (argsList.at(MallocObjectArg::fun) == "calloc"))
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

    if (map.size() == 0)
        result_.add(qMakePair(IViewType::source , QString("There are no possible memory leaks")));
}

void MemoryModel::leakToSourceCode_(const QMap<QString, MallocObject>& map, const QString& elf)
{
    if (map.size() == 0)
        return;

    const QRegExp rx("[:]");

    Result result;

    for (auto iter : map)
    {
        QString cmd("addr2line -e " + elf + " " + iter.address + " > " + fileLog_);

        if (system(cmd.toStdString().c_str()) != 0)
            throw QString("Warning: Can't process addr2line for: " + elf);

        QFile addr2LineFile(fileLog_);
        if (!addr2LineFile.open(QIODevice::ReadOnly | QFile::Text))
            throw QString("Warning: Can't open file: " + addr2LineFile.errorString());

        QTextStream inTextStream(&addr2LineFile);
        while (!inTextStream.atEnd())
        {
            auto line     = inTextStream.readLine();
            auto argsList = line.split(rx, QString::SkipEmptyParts);

            result.add(qMakePair(IViewType::source, line));

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
                result.add(qMakePair(IViewType::leak, QString(QString::number(cnt) + "  " + tmp)));
            else
                result.add(qMakePair(IViewType::line, QString(QString::number(cnt) + "  " + tmp)));
         }

         cnt++;
    }

    // Close the source file
    file.close();
}
