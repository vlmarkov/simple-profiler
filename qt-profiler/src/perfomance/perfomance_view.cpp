#include "include/model.h"
#include "include/controller.h"
#include "include/perfomance/perfomance_view.h"

#include <QDebug>


PerfomanceView::PerfomanceView(Model&        model,
                               Controller&   controller,
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
