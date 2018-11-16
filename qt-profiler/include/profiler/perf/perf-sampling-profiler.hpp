#pragma once

#include <include/i-profiler.hpp>


class PerfSamplingProfiler : public IProfiler
{
    public:
        PerfSamplingProfiler()  = default;
        ~PerfSamplingProfiler() = default;

        void run(const QString& pathTo, BaseProfiler& baseProfiler) override;
};
