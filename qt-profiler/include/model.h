#ifndef MODEL_H
#define MODEL_H

#include <QString>

#include "result.h"
#include "observer.h"
#include "view_type.h"


// Abstract base class
class Model : public Observable
{
    public:
        Model();
        virtual ~Model();

        virtual void processRequest(const QString& str) = 0;
        virtual Result getResult() = 0;
};

#endif // MODEL_H
