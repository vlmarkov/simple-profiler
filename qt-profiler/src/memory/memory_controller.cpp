#include "include/memory/memory_controller.h"


MemoryController::MemoryController(Model& model) noexcept : model_(model) { }

void MemoryController::requestProcess(const QString& request)
{
    model_.requestProcess(request);
}
