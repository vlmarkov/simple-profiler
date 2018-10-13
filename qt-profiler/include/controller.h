#pragma once

#include <QString>

#include "include/observer.h"


// Pure abstract base class
class Controller : public Observable
{
    public:
        Controller() = default;
        virtual ~Controller() = default;

        virtual void requestProcess(const QString& request) = 0;
};