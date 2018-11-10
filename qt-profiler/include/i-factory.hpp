#pragma once

#include <memory>

#include <include/i-profiler.hpp>


enum class IFactoryType { perf_event, perf_sample, mem_leak };


// This is a interface class
class IFactory
{
    public:
        static auto createProfiler(const IFactoryType& type) -> std::shared_ptr<IProfiler>;
};
