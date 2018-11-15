#pragma once

#include <QVector>


enum class IObserverEvent { fail, succses, sudoRight };

// This is an interface class
class IObserver
{
    public:
        IObserver() = default;
        virtual ~IObserver() = default;

        virtual void update(const IObserverEvent& event) = 0;
};
