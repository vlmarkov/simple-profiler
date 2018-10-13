#include "include/perfomance/perfomance_controller.h"


PerfomanceController::PerfomanceController(Model& model) noexcept : model_(model) { }

void PerfomanceController::requestProcess(const QString& request)
{
    model_.requestProcess(request);
}
