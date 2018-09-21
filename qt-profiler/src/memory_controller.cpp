#include "include/memory_controller.h"


MemoryController::MemoryController(Model* model) :
    model_(model)
{
    ;
}

MemoryController::~MemoryController()
{
    ;
}

void MemoryController::processRequest(const QString& str)
{
    this->model_->processRequest(str);
}
