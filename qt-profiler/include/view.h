#pragma once

#include "event.h"
#include "observer.h"


// Pure abstract base class
class View : public Observer
{
    public:
        View() = default;
        virtual ~View() = 0;

        virtual void update(const Event& event) = 0;
};
