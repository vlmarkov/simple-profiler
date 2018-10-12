#include "include/memory_controller.h"


MemoryController::MemoryController(Model& model) noexcept : model_(model)
{
    ;
}

MemoryController::~MemoryController()
{
    ;
}

void MemoryController::processRequest(const QString& request)
{
    model_.processRequest(request);
}
