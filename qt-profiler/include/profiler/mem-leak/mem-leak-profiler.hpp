#pragma once

#include <QString>

#include <include/i-profiler.hpp>
#include <include/base-profiler.hpp>


class MemLeakProfiler : public IProfiler
{
    public:
        MemLeakProfiler()  = default;
        ~MemLeakProfiler() = default;

        void run(const QString& pathToFile, BaseProfiler& baseProf) override;
};
