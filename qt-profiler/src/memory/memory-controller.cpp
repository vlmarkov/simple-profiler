#include <include/memory/memory-controller.hpp>


MemoryController::MemoryController(IModel& model) noexcept : model_(model)
{
    ;
}

void MemoryController::requestProcess(const QString& request)
{
    model_.requestProcess(request);
}
