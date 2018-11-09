#pragma once

#include <memory>

#include <include/i-profiler.hpp>


enum class IFactoryType { perf, malloc };


// This is a interface class
class IFactory
{
    protected:
        virtual ~IFactory() = default;
        IFactory& operator=(const IFactory&) = delete;

    public:
        virtual std::shared_ptr<IProfiler> createProfiler(const IFactoryType& type) = 0;
};
