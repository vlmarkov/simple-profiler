#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-controller.hpp>

#include <unistd.h>
#include <sys/types.h>


PerfController::PerfController(IModel& model) noexcept : model_(model)
{
    ;
}

void PerfController::requestProcess(const QString& request)
{
    if (::getuid() != 0 && ::geteuid() != 0)
        Observable::notify(Event::sudoRight);
    else
        model_.requestProcess(request);
}
