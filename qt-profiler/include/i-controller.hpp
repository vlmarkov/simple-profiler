#pragma once

#include <QString>

#include <include/observable.hpp>


// This is an interface class
class IController : public Observable
{
    public:
        IController() = default;
        virtual ~IController() = default;

        virtual void requestProcess(const QString& request) = 0;
};
