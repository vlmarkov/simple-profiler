#pragma once

#include <QVector>

#include "include/event.h"


class Observer
{
    public:
        Observer() = default;
        virtual ~Observer() = default;

        virtual void update(const Event& event) = 0;
};

class Observable
{
    public:
        Observable() = default;
        ~Observable() = default;

        void add(Observer& observer) noexcept;
        void notify(const Event& event);

    private:
        QVector<Observer*> observers_;
};