#pragma once

#include <QVector>

#include <include/i-observer.hpp>


class Observable
{
    public:
        Observable() = default;
        ~Observable() = default;

        void add(IObserver& observer) noexcept;
        void notify(const Event& event);

    private:
        QVector<IObserver*> observers_;
};
