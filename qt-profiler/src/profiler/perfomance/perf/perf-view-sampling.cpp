#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>
#include <include/profiler/perfomance/perf/perf-view-sampling.hpp>


PerfViewSampling::PerfViewSampling(IModel& model, IController& controller, BaseProfiler& baseProf):
    model_(model), controller_(controller), baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void PerfViewSampling::update(const Event& event)
{
    return; // TODO
}
