#pragma once

#include <QVector>

#include <include/event.hpp>


// This is an interface class
class IObserver
{
    public:
        IObserver() = default;
        virtual ~IObserver() = default;

        virtual void update(const Event& event) = 0;
};
