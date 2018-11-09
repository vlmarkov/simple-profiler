#pragma once

#include <QString>

#include <include/result.hpp>
#include <include/observable.hpp>


// This is an interface class
class IModel : public Observable
{
    public:
        IModel() = default;
        virtual ~IModel() = default;

        virtual void requestProcess(const QString& request) = 0;
        virtual Result getResult() = 0;
};
