#pragma once

#include <QString>

#include "include/result.h"
#include "include/observer.h"


// Pure abstract base class
class Model : public Observable
{
    public:
        Model() = default;
        virtual ~Model() = default;

        virtual void processRequest(const QString& request) = 0;
        virtual Result getResult() = 0;
};
