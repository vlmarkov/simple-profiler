#pragma once

#include <QString>

#include <include/i-profiler.hpp>


class PerfProfilerEvents : public IProfiler
{
    public:
        PerfProfilerEvents()  = default;
        ~PerfProfilerEvents() = default;

        void run(const QString& pathToFile, BaseProfiler& baseProfiler) override;
};

