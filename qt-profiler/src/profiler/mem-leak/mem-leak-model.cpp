#include <stdlib.h>
#include <boost/process.hpp>

#include <include/profiler/exception.hpp>
#include <include/profiler/mem-leak/mem-leak-model.hpp>
#include <include/profiler/mem-leak/mem-leak-utils.hpp>


MemLeakModel::MemLeakModel() noexcept :
    fileLib_(QString("lib_hook_malloc.so")),
    fileLogLeak_(QString("log-leak.txt")),
    fileLogAddr_(QString("log-addr.txt"))
{
    ;
}

MemLeakModel::~MemLeakModel()
{
    try
    {
        ::remove(this->fileLogLeak_.toUtf8().constData());
        ::remove(this->fileLogAddr_.toUtf8().constData());
    }
    catch (...)
    {
        ; // Do not propagate exception
    }
}

void MemLeakModel::process(const QString& pathTo)
{
    auto event = IObserverEvent::fail;

    try {
        QString cmd("LD_PRELOAD=./" + this->fileLib_ + " " + pathTo + " 2> " + this->fileLogLeak_);
        if (system(cmd.toStdString().c_str()) != 0)
            throw Exception(QString("Warning: Can't run: " + pathTo));

        FileReaderMemLeak fileLeak(this->fileLogLeak_);
        auto leaks = fileLeak.readMemLeaks();
        if (leaks.size() == 0)
        {
            this->result_.add(qMakePair(IViewType::source, QString("There are no possible memory leaks")));
        }
        else
        {
            for (auto iter : leaks)
            {
                QString cmd("addr2line -e " + pathTo + " " + iter.address);
                boost::process::system(cmd.toStdString(), boost::process::std_out > this->fileLogAddr_.toStdString());

                FileReaderSource fileSource(this->fileLogAddr_);
                fileSource.readSource(this->result_);

                if (remove(this->fileLogAddr_.toUtf8().constData()) != 0)
                    throw Exception(QString("Warning: Can't delete file: " + this->fileLogAddr_));
            }
        }

        event = IObserverEvent::succses;
    }
    catch (Exception& exception)
    {
        this->result_.add(qMakePair(IViewType::error , QString(exception.what())));
    }
    catch (std::exception& exception)
    {
        this->result_.add(qMakePair(IViewType::error , QString(exception.what())));
    }
    catch (...)
    {
        this->result_.add(qMakePair(IViewType::error , QString("Unknown error")));
    }

    Observable::notify(event);
}

Result MemLeakModel::getResult() noexcept
{
    return result_;
}
