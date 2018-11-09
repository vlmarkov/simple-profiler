#include <include/profiler/memory/memory-factory.hpp>
#include <include/profiler/memory/memory-profiler.hpp>


std::shared_ptr<IProfiler> MemoryFactory::createProfiler(const IFactoryType& type)
{
    return std::make_shared<MemoryProfiler>();
}
