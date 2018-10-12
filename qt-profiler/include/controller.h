#pragma once

#include <QString>

#include "observer.h"


// Pure abstract base class
class Controller : public Observable
{
    public:
        Controller() = default;
        virtual ~Controller() = 0;

        virtual void processRequest(const QString& str) = 0;
};
