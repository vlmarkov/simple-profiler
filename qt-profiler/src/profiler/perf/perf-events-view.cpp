#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>
#include <include/profiler/perf/perf-events-view.hpp>


PerfEventsView::PerfEventsView(IModel& model, IController& controller, BaseProfiler& baseProf):
    model_(model), controller_(controller), baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void PerfEventsView::update(const IObserverEvent& event)
{
    switch (event)
    {
        case IObserverEvent::sudoRight:
        {
            Result result;
            result.add(qMakePair(IViewType::error , QString("You don't have a sudo right!")));
            baseProf_.setResult(this->toHtml_(result));
            break;
        }
        case IObserverEvent::fail:
        {
            baseProf_.setResult(this->toHtml_(model_.getResult()));
            break;
        }
        case IObserverEvent::succses:
        {
            baseProf_.setResult(this->toHtml_(model_.getResult()));
            break;
        }
        default:
        {
            break;
        }
    }
}

QString PerfEventsView::toHtml_(Result result) noexcept
{
    QVector<QString> colors;
    colors.push_back("<font color=\"red\">");

    QString htmlStr;
    htmlStr += "<section><p></p><p></p>";
    htmlStr += "<table width=\"800\" border=2 bgcolor=\"white\">";

    for (auto iter : result.get())
    {
        switch (iter.first)
        {
            case IViewType::value:
            {
                htmlStr += "<tr><td>" + (iter.second) + "</td></tr>";
                break;
            }
            case IViewType::error:
            {
                htmlStr += "<tr><td>" + colors.at(0) + (iter.second) + "</td></tr>";
                break;
            }
            default:
            {
                break;
            }
        }
    }

    htmlStr += "</table>";
    htmlStr += "<p></p></section>";

    return htmlStr;
}
