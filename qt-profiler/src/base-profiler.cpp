#include <include/i-factory.hpp>
#include <include/base-profiler.hpp>
#include <include/profiler/exception.hpp>


void BaseProfiler::runMemoryCheck(const QString& pathTo)
{
    try {
        auto profiler = IFactory::createProfiler(IFactoryType::mem_leak);
        profiler->run(pathTo, *this);
    }
    catch (Exception& exception)
    {
        this->setResult(QString(exception.what())+ QString::number(exception.code()));
    }
    catch (std::exception& exception)
    {
        this->setResult(QString(exception.what()));
    }
    catch (...)
    {
        this->setResult(QString("Caught unexpected exception"));
    }
}

void BaseProfiler::runPerfomanceSampleCheck(const QString& pathTo)
{
    try
    {
        auto profiler = IFactory::createProfiler(IFactoryType::perf_sample);
        profiler->run(pathTo, *this);
    }
    catch (Exception& exception)
    {
        this->setResult(QString(exception.what())+ QString::number(exception.code()));
    }
    catch (std::exception& exception)
    {
        this->setResult(QString(exception.what()));
    }
    catch (...)
    {
        this->setResult(QString("Caught unexpected exception"));
    }
}

void BaseProfiler::runPerfomanceEventCheck(const QString& pathTo)
{
    try
    {
        auto profiler = IFactory::createProfiler(IFactoryType::perf_event);
        profiler->run(pathTo, *this);
    }
    catch (Exception& exception)
    {
        this->setResult(QString(exception.what())+ QString::number(exception.code()));
    }
    catch (std::exception& exception)
    {
        this->setResult(QString(exception.what()));
    }
    catch (...)
    {
        this->setResult(QString("Caught unexpected exception"));
    }
}

QString BaseProfiler::getResult() noexcept
{
    return result_;
}

void BaseProfiler::setResult(const QString& string) noexcept
{
    result_ = string;
    emit resultChanged();
}
