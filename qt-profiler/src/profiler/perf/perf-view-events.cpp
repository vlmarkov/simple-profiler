#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>
#include <include/profiler/perf/perf-view-events.hpp>


PerfViewEvents::PerfViewEvents(IModel& model, IController& controller, BaseProfiler& baseProf):
    model_(model), controller_(controller), baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void PerfViewEvents::update(const IObserverEvent& event)
{
    return; // TODO
}
