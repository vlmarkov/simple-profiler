#include "include/observer.h"

#include <QString>


Observer::Observer()
{
    ;
}

Observer::~Observer()
{
    ;
}

/*****************************************************************************/

Observable::Observable()
{
    ;
}

Observable::~Observable()
{
    ;
}

void Observable::addObserver(Observer* observer)
{
    if (!observer)
        throw(QString("Can't add observer"));

    this->observers_.push_back(observer);
}

void Observable::notifyObserver(const Event& event)
{
    for (auto i : this->observers_)
        i->update(event);
}
