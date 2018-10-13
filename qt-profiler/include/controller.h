#pragma once

#include <QString>

#include "observer.h"


// Pure abstract base class
class Controller : public Observable
{
    public:
        Controller() = default;
        virtual ~Controller() = default;

        virtual void processRequest(const QString& request) = 0;
};
