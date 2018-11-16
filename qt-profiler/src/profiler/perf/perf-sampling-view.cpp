#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>
#include <include/profiler/perf/perf-sampling-view.hpp>


PerfSamplingView::PerfSamplingView(IModel& model, IController& controller, BaseProfiler& baseProf):
    model_(model), controller_(controller), baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void PerfSamplingView::update(const IObserverEvent& event)
{
    switch (event)
    {
        case IObserverEvent::sudoRight:
        {
            Result result;
            result.add(qMakePair(IViewType::error , QString("You don't have a sudo right!")));
            baseProf_.setResult(toHtml_(result));
            break;
        }
        case IObserverEvent::fail:
        {
            baseProf_.setResult(toHtml_(model_.getResult()));
            break;
        }
        case IObserverEvent::succses:
        {
            baseProf_.setResult(toHtml_(model_.getResult()));
            break;
        }
        default:
        {
            break;
        }
    }
}

QString PerfSamplingView::toHtml_(Result result) noexcept
{
    QString htmlStr;

    htmlStr += "<section><p></p><p></p>";
    htmlStr += "<table border = 1 bgcolor=\"white\">";
    htmlStr += "<tr><th>**** ****</th><th>**** ****</th></tr>";

    for (auto i : result.get())
    {
        if (i.first == IViewType::error)
            htmlStr += "<tr><td style='color:red'>" + (i.second) + "</td><td></td></tr>";
        else
            htmlStr += "<tr><td>" + (i.second) + "</td><td> source code line</td></tr>";
    }

    htmlStr += "</table>";
    htmlStr += "<p></p></section>";

    return htmlStr;
}
