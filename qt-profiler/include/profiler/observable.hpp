#pragma once

#include <QVector>

#include <include/profiler/i-observer.hpp>


class Observable
{
    private:
        QVector<IObserver*> observers_;

    public:
        Observable() = default;
        ~Observable() = default;

        void add(IObserver& observer);
        void notify(const IObserverEvent& event);
};
