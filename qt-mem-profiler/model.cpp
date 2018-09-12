#include <QFile>
#include <QTextStream>

#include <stdlib.h>

#include "model.h"


Model::Model()
{
    libFileName_ = "lib_hook_malloc.so";
    logFileName_ = "log";
}

Model::~Model()
{
    ;
}

void Model::runMemProfiler(const QString& str)
{
    QMap<QString, MallocObject> leakMap;

    try {
        this->collectMallocUsage_(str);

        this->readMallocUsage_(leakMap);
        if (leakMap.size() == 0)
        {
            result_ = "There are no possible memory leaks";
            this->notifyObserver(Event::FindFiles);
            return;
        }

        this->leakToSourceCode_(leakMap, str);
    }
    catch (QString& err)
    {
        result_ = err;
        this->notifyObserver(Event::FindFiles);
        return;
    }

    this->notifyObserver(Event::FindFiles);
}

QString Model::getMemProfillerResult(void)
{
    return result_;
}

void Model::collectMallocUsage_(const QString& elfName)
{
    QString arg;

    arg += "LD_PRELOAD=";
    arg += "./";
    arg += this->libFileName_;
    arg += " ";
    arg += elfName;
    arg += " 2>";
    arg += this->logFileName_;

    if (system(arg.toStdString().c_str()) != 0)
    {
        throw QString("Warning: Can't collect malloc usage for: " + elfName);
    }
}

void Model::readMallocUsage_(QMap<QString, MallocObject>& mallocMap)
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
            mallocMap.remove(key);
        }

        if (argsList.at(MallocObjectArg::fun) == "malloc" || argsList.at(MallocObjectArg::fun) == "calloc")
        {
            auto key = argsList.at(MallocObjectArg::ptr);
            auto value = MallocObject(argsList.at(MallocObjectArg::addr),
                                      argsList.at(MallocObjectArg::fun),
                                      argsList.at(MallocObjectArg::size),
                                      argsList.at(MallocObjectArg::ptr));

            mallocMap.insert(key, value);
        }

        if (argsList.at(MallocObjectArg::fun) == "realloc")
        {
            auto key = argsList.at(MallocObjectArg::ptr);
            auto value = MallocObject(argsList.at(MallocObjectArg::addr),
                                      argsList.at(MallocObjectArg::fun),
                                      argsList.at(MallocObjectArg::size),
                                      argsList.at(MallocObjectArg::ptr));

            mallocMap.remove(key);
            mallocMap.insert(key, value);
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

void Model::leakToSourceCode_(const QMap<QString, MallocObject>& leakMap, const QString& elfName)
{
    const QRegExp rx("[:]");

    QString leakLog;

    for (auto it = leakMap.begin(); it != leakMap.end(); ++it)
    {
        QString arg;
        arg += ("addr2line -e ");
        arg += elfName;
        arg += " ";
        arg += it.value()._addr;
        arg += " > ";
        arg += this->logFileName_;

        if (system(arg.toStdString().c_str()) != 0)
        {
            throw QString("Warning: Can't process addr2line for: " + elfName);
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

            leakLog += line + "\n";

            this->readSourceCode_(argsList.at(0), argsList.at(1).toInt(), leakLog);
        }

        // Close the file
        addr2LineFile.close();

        // Delete temporal log file
        if (remove(this->logFileName_.toUtf8().constData()) != 0)
        {
            throw QString("Warning: Can't delete file: " + this->logFileName_);
        }
    }

    this->result_ = leakLog;
}

void Model::readSourceCode_(const QString& pathTo, const int line, QString& leakLog)
{
    // Get path to source
    QFile srcFile(pathTo);
    if (!srcFile.open(QIODevice::ReadOnly | QFile::Text))
    {
        throw QString("Warning: Can't open file: " + srcFile.errorString());
    }

    leakLog += "\n";

    int cnt = 1;
    QTextStream in(&srcFile);
    while (!in.atEnd())
    {
        auto str = in.readLine();

        if ((line - 3 < cnt) && (line + 3 > cnt))
        {
            if (line == cnt)
                leakLog += "!-->";
            else
                leakLog += "    ";

            leakLog += QString::number(cnt);
            leakLog += " ";
            leakLog += str;
            leakLog += "\n";
         }
         cnt++;
    }
    leakLog += "\n";

    // Close the file
    srcFile.close();
}
