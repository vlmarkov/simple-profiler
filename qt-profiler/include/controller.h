#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>

#include "observer.h"


// Abstract base class
class Controller : public Observable
{
    public:
        Controller();
        virtual ~Controller() = 0;

        virtual void processRequest(const QString& str) = 0;
};

#endif // CONTROLLER_H
