#include "controller.h"
#include "event.h"

Controller::Controller(Model* model) : model_(model)
{
    ;
}

Controller::~Controller()
{
    ;
}

void Controller::processRequest(QString& str)
{
    this->model_->runMemProfiler(str);
}
