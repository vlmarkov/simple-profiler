#ifndef VIEW_H
#define VIEW_H

#include "event.h"
#include "observer.h"


// Abstract base class
class View : public Observer
{
    public:
        View();
        virtual ~View() = 0;

        virtual void update(const Event& event) = 0;
};

#endif // VIEW_H
