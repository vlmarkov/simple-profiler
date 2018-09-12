#ifndef OBSERVER_H
#define OBSERVER_H

#include <QVector>

#include "event.h"


class Observer
{
    public:
        Observer();
        ~Observer();

        virtual void update(const Event& event) = 0;
};

class Observable
{
    public:
        Observable();
        ~Observable();

        void addObserver(Observer* observer);
        void notifyObserver(const Event& event);

    private:
        QVector<Observer*> observers_;
};

#endif // OBSERVER_H
