#include <include/i-factory.hpp>
#include <include/base-profiler.hpp>
#include <include/profiler/exception.hpp>


void BaseProfiler::runMemoryCheck(const QString& pathTo)
{
    try {
        auto profiler = IFactory::createProfiler(IFactoryType::mem_leak);
        profiler->run(pathTo, *this);
    }
    catch (...)
    {
        this->setResult(QString("Can't run profiler"));
    }
}

void BaseProfiler::runPerfomanceSampleCheck(const QString& pathTo)
{
    try
    {
        auto profiler = IFactory::createProfiler(IFactoryType::perf_sample);
        profiler->run(pathTo, *this);
    }
    catch (...)
    {
        this->setResult(QString("Can't run profiler"));
    }
}

void BaseProfiler::runPerfomanceEventCheck(const QString& pathTo)
{
    try
    {
        auto profiler = IFactory::createProfiler(IFactoryType::perf_event);
        profiler->run(pathTo, *this);
    }
    catch (...)
    {
        this->setResult(QString("Can't run profiler"));
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
