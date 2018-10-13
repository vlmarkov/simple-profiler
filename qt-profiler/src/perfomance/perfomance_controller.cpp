#include "include/perfomance/perfomance_controller.h"


PerfomanceController::PerfomanceController(Model& model) noexcept : model_(model) { }

void PerfomanceController::processRequest(const QString& request)
{
    model_.processRequest(request);
}
