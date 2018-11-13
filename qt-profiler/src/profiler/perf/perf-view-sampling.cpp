#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>
#include <include/profiler/perf/perf-view-sampling.hpp>


#include <QDebug>

PerfViewSampling::PerfViewSampling(IModel& model, IController& controller, BaseProfiler& baseProf):
    model_(model), controller_(controller), baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void PerfViewSampling::update(const Event& event)
{
    switch (event)
    {
        case Event::sudoRight:
        {
            Result result;
            result.add(qMakePair(IViewType::error , QString("You don't have a sudo right!")));
            baseProf_.setResult(toHtml_(result));
            break;
        }
        case Event::fail:
        {
            baseProf_.setResult(toHtml_(model_.getResult()));
            break;
        }
        case Event::succses:
        {
            baseProf_.setResult(toHtml_(model_.getResult()));
            break;
        }
    }
}

QString PerfViewSampling::toHtml_(Result result) noexcept
{
    QString htmlStr;

    htmlStr += "<section><p>";
    for (auto i : result.get())
        htmlStr += "<p>" + i.second + "</p>";
    htmlStr += "</p></section>";

    return htmlStr;
}
