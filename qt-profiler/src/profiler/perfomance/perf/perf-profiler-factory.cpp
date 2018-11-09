#include <perf-profiler-events.hpp>
#include <perf-profiler-factory.hpp>
#include <perf-profiler-sampling.hpp>


std::shared_ptr<IProfiler> PerfProfilerFactory::createCacheProfiler()
{
    std::vector<uint32_t> hw_cache = {
        PERF_COUNT_HW_CACHE_REFERENCES,
        PERF_COUNT_HW_CACHE_MISSES
    };

    return std::make_shared<PerfProfilerEvents>(hw_cache);
}

std::shared_ptr<IProfiler> PerfProfilerFactory::createBranchProfiler()
{
    std::vector<uint32_t> hw_branch = {
        PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
        PERF_COUNT_HW_BRANCH_MISSES
    };

    return std::make_shared<PerfProfilerEvents>(hw_branch);
}

std::shared_ptr<IProfiler> PerfProfilerFactory::createInstructionProfiler()
{
    std::vector<uint32_t> hw_cpu = {
        PERF_COUNT_HW_CPU_CYCLES,
        PERF_COUNT_HW_INSTRUCTIONS
    };
            
    return std::make_shared<PerfProfilerEvents>(hw_cpu);
}


std::shared_ptr<IProfiler> PerfProfilerFactory::createSamplingProfiler()
{
    return std::make_shared<PerfProfilerSampling>();
}
