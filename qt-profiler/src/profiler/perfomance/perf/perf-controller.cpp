#include <include/profiler/perfomance/perf/exception.hpp>
#include <include/profiler/perfomance/perf/perf-controller.hpp>

#include <unistd.h>
#include <sys/types.h>


PerfController::PerfController(IModel& model) noexcept : model_(model)
{
    ;
}

void PerfController::requestProcess(const QString& request)
{
    if (::getuid() != 0 && ::geteuid() != 0)
        throw Exception("You don't have a sudo right!");

    model_.requestProcess(request);
}
