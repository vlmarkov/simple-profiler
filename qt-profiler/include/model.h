#pragma once

#include <QString>

#include "result.h"
#include "observer.h"


// Pure abstract base class
class Model : public Observable
{
    public:
        Model() = default;
        virtual ~Model() = default;

        virtual void processRequest(const QString& request) = 0;
        virtual Result getResult() = 0;
};
