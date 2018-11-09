#include <include/perfomance/perfomance-controller.hpp>


PerfomanceController::PerfomanceController(IModel& model) noexcept : model_(model)
{
    ;
}

void PerfomanceController::requestProcess(const QString& request)
{
    model_.requestProcess(request);
}
