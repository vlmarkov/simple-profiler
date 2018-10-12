#include "include/observer.h"

#include <QString>


Observer::~Observer()
{
    ;
}

/*****************************************************************************/

Observable::~Observable()
{
    ;
}

void Observable::add(Observer* observer)
{
    if (!observer)
        throw(QString("Can't add observer"));

    observers_.push_back(observer);
}

void Observable::notify(const Event& event)
{
    for (auto i : observers_)
        i->update(event);
}
