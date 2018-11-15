#include <include/i-factory.hpp>
#include <include/profiler/exception.hpp>
#include <include/profiler/mem-leak/mem-leak-profiler.hpp>
#include <include/profiler/perf/perf-profiler-events.hpp>
#include <include/profiler/perf/perf-profiler-sampling.hpp>


auto IFactory::createProfiler(const IFactoryType& type) -> std::shared_ptr<IProfiler>
{
    switch (type)
    {
        case IFactoryType::mem_leak:
            return std::make_shared<MemLeakProfiler>();
        case IFactoryType::perf_event:
            return std::make_shared<PerfProfilerEvents>();
        case IFactoryType::perf_sample:
            return std::make_shared<PerfProfilerSampling>();
    }
    throw Exception("Failed to create profiler");
}
