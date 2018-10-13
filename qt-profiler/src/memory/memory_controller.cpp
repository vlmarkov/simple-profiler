#include "include/memory/memory_controller.h"


MemoryController::MemoryController(Model& model) noexcept : model_(model) { }

void MemoryController::processRequest(const QString& request)
{
    model_.processRequest(request);
}
