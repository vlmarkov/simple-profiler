#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>

#include "model.h"


class Controller : public Observable
{
    public:
        Controller(Model* model);
        ~Controller();

        void processRequest(QString& str);

    private:
        Model* model_;
};

#endif // CONTROLLER_H
