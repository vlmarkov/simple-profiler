#include <include/profiler/mem-leak/mem-leak-controller.hpp>


MemLeakController::MemLeakController(IModel& model) noexcept : model_(model)
{
    ;
}

void MemLeakController::requestProcess(const QString& request)
{
    model_.requestProcess(request);
}
