#include <include/i-factory.hpp>
#include <include/profiler/memory/memory-profiler.hpp>
#include <include/profiler/perfomance/perf/profiler-events.hpp>
#include <include/profiler/perfomance/perf/profiler-sampling.hpp>


auto IFactory::createProfiler(const IFactoryType& type) -> std::shared_ptr<IProfiler>
{
    switch (type)
    {
        case IFactoryType::mem_leak:
            return std::make_shared<MemoryProfiler>();
        case IFactoryType::perf_event:
            return std::make_shared<PerfProfilerEvents>();
        case IFactoryType::perf_sample:
            return std::make_shared<PerfProfilerSampling>();
    }
    // TODO: exception throw
}
