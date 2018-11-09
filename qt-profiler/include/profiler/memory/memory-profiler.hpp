#pragma once

#include <QString>

#include <include/i-profiler.hpp>
#include <include/base-profiler.hpp>


class MemoryProfiler : public IProfiler
{
    public:
        MemoryProfiler()  = default;
        ~MemoryProfiler() = default;

        void run(const QString& request, BaseProfiler& baseProf) override;
};
