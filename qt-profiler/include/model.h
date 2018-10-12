#pragma once

#include <QString>

#include "result.h"
#include "observer.h"
#include "view_type.h"


// Pure abstract base class
class Model : public Observable
{
    public:
        Model() = default;
        virtual ~Model();

        virtual void processRequest(const QString& str) = 0;
        virtual Result getResult() = 0;
};
