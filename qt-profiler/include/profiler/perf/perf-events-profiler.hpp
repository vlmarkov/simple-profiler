#pragma once

#include <include/i-profiler.hpp>


class PerfEventsProfiler : public IProfiler
{
    public:
        PerfEventsProfiler()  = default;
        ~PerfEventsProfiler() = default;

        void run(const QString& pathTo, BaseProfiler& baseProfiler) override;
};

