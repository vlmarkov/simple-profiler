#pragma once

#include <QString>

#include <include/base-profiler.hpp>


// This is a interface class
class IProfiler
{
    protected:
        virtual ~IProfiler() = default;
        IProfiler& operator=(const IProfiler&) = delete;

    public:
        virtual void run(const QString& request, BaseProfiler& baseProfiler) = 0;
};
