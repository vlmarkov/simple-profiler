#pragma once

#include <QString>

#include <include/profiler/result.hpp>
#include <include/profiler/observable.hpp>


// This is an interface class
class IModel : public Observable
{
    public:
        IModel() = default;
        virtual ~IModel() = default;

        virtual void process(const QString& pathTo) = 0;
        virtual Result getResult() = 0;
};
