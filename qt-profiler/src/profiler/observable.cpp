#include <QString>

#include <include/profiler/observable.hpp>


void Observable::add(IObserver& observer) noexcept
{
    observers_.push_back(&observer);
}

void Observable::notify(const IObserverEvent& event)
{
    for (auto i : observers_)
        i->update(event);
}
