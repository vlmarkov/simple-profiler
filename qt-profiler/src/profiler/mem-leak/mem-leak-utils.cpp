#include <QTextStream>

#include <include/profiler/i-view.hpp>
#include <include/profiler/mem-leak/mem-leak-utils.hpp>


MemLeakObj::MemLeakObj(QString addr,
                       QString fun,
                       QString sz,
                       QString ptr) noexcept :
    address(addr),
    function(fun),
    size(sz),
    pointer(ptr)
{
    ;
}

FileReader::FileReader(const QString& pathTo)
{
    this->file_.setFileName(pathTo);
    if (!this->file_.open(QIODevice::ReadOnly | QFile::Text))
        throw QString("Warning: Can't open file: " + pathTo);
}

FileReader::~FileReader()
{
    try
    {
        this->file_.close();
    }
    catch (...)
    {
        ; // Do not propagate exception
    }
}

FileReaderMemLeak::FileReaderMemLeak(const QString& pathTo) : FileReader(pathTo)
{
    ;
}

QMap<QString, MemLeakObj> FileReaderMemLeak::readMemLeaks()
{
    const QRegExp regExp("[ ]");

    QMap<QString, MemLeakObj> map;

    QTextStream in(&this->file_);
    while (!in.atEnd())
    {
        // Read line and split it
        auto argsList = in.readLine().split(regExp, QString::SkipEmptyParts);

        if (argsList.at(MemLeakObjArg::fun) == "free")
        {
            auto key = argsList.at(MemLeakObjArg::ptr);
            map.remove(key);
        }

            if ((argsList.at(MemLeakObjArg::fun) == "malloc") ||
                (argsList.at(MemLeakObjArg::fun) == "calloc"))
            {
                auto key = argsList.at(MemLeakObjArg::ptr);
                auto value = MemLeakObj(argsList.at(MemLeakObjArg::addr),
                                        argsList.at(MemLeakObjArg::fun),
                                        argsList.at(MemLeakObjArg::size),
                                        argsList.at(MemLeakObjArg::ptr));

                map.insert(key, value);
            }

            if (argsList.at(MemLeakObjArg::fun) == "realloc")
            {
                auto key   = argsList.at(MemLeakObjArg::ptr);
                auto value = MemLeakObj(argsList.at(MemLeakObjArg::addr),
                                        argsList.at(MemLeakObjArg::fun),
                                        argsList.at(MemLeakObjArg::size),
                                        argsList.at(MemLeakObjArg::ptr));

                map.remove(key);
                map.insert(key, value);
        }
    }

    return map;
}

FileReaderSource::FileReaderSource(const QString& pathTo) : FileReader(pathTo)
{
    ;
}

void FileReaderSource::readSource(Result& result)
{
    const QRegExp regExpSrcLine("[:]");
    const QRegExp regExpSrcFile("[/]");

    QTextStream in(&this->file_);
    while (!in.atEnd())
    {
        auto line     = in.readLine();
        auto lineList = line.split(regExpSrcLine, QString::SkipEmptyParts);
        auto fileList = line.split(regExpSrcFile, QString::SkipEmptyParts);

        result.add(qMakePair(IViewType::source, fileList.value(fileList.length() - 1)));

        FileReaderSourceLine srcLineReader(lineList.at(0));
        srcLineReader.readSourceLine(lineList.at(1).toInt(), result);
    }
}

FileReaderSourceLine::FileReaderSourceLine(const QString& pathTo) : FileReader(pathTo)
{
    ;
}

void FileReaderSourceLine::readSourceLine(const int ln, Result& result)
{
    auto cnt = 1;

    QTextStream in(&this->file_);
    while (!in.atEnd())
    {
        auto tmp = in.readLine();

        if ((ln - 3 < cnt) && (ln + 3 > cnt))
        {
            if (ln == cnt)
                result.add(qMakePair(IViewType::leak, QString(QString::number(cnt) + "  " + tmp)));
            else
                result.add(qMakePair(IViewType::line, QString(QString::number(cnt) + "  " + tmp)));
         }

         cnt++;
    }
}
