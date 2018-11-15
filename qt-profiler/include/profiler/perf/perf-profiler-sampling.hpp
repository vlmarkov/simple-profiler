#pragma once

#include <QString>

#include <include/i-profiler.hpp>


class PerfProfilerSampling : public IProfiler
{
    public:
        PerfProfilerSampling()  = default;
        ~PerfProfilerSampling() = default;

        void run(const QString& pathTo, BaseProfiler& baseProfiler) override;
};
