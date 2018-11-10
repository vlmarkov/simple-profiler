#include <include/i-factory.hpp>
#include <include/base-profiler.hpp>


void BaseProfiler::runMemoryCheck(const QString& pathToFile)
{
    auto profiler = IFactory::createProfiler(IFactoryType::mem_leak);
    profiler->run(pathToFile, *this);
}

void BaseProfiler::runPerfomanceCheck(const QString& pathToFile)
{
    //auto profiler = IFactory::createProfiler(IFactoryType::perf_sample);
    auto profiler = IFactory::createProfiler(IFactoryType::perf_event);
    profiler->run(pathToFile, *this);
}

QString BaseProfiler::getResult() noexcept
{
    return result_;
}

void BaseProfiler::setResult(const QString& rhs) noexcept
{
    result_ = rhs;
    emit resultChanged();
}
