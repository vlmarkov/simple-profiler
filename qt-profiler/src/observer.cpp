#include "include/observer.h"

#include <QString>


void Observable::add(Observer& observer) noexcept
{
    observers_.push_back(&observer);
}

void Observable::notify(const Event& event)
{
    for (auto i : observers_)
        i->update(event);
}
