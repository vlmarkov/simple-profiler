#include <include/profiler/mem-leak/mem-leak-controller.hpp>


MemLeakController::MemLeakController(IModel& model) noexcept : model_(model)
{
    ;
}

void MemLeakController::process(const QString& pathTo)
{
    model_.process(pathTo);
}
