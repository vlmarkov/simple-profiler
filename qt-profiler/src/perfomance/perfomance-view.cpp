#include <include/i-model.hpp>
#include <include/i-controller.hpp>
#include <include/perfomance/perfomance-view.hpp>


PerfomanceView::PerfomanceView(IModel&       model,
                               IController&  controller,
                               BaseProfiler& baseProf):
    model_(model),
    controller_(controller),
    baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void PerfomanceView::update(const Event& event)
{
    return; // TODO
}
