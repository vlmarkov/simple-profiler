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
    auto firstHotSpot = true;
    auto i = 0, cnt = 0;

    QVector<QString> colors;
    colors.push_back("<font color=\"red\">");
    colors.push_back("<font color=\"orange\">");
    colors.push_back("<font color=\"blue\">");

    QString htmlStr;
    htmlStr += "<section><p></p><p></p>";
    htmlStr += "<table width=\"800\" border=2 bgcolor=\"white\">";

    for (auto iter : result.get())
    {
        switch (iter.first)
        {
            case IViewType::source:
            {
                htmlStr += "</table>";

                if (firstHotSpot)
                {
                    firstHotSpot = false;
                }
                else
                {
                    htmlStr += "<p></p>";
                }

                htmlStr += "<table width=\"800\" border=2 bgcolor=\"white\">";
                htmlStr += "<tr><td> File: " + (iter.second) + "</td></tr>";
                break;
            }
            case IViewType::value:
            {
                htmlStr += "<tr><td>";
                auto idx = (i > colors.size() - 1) ? (colors.size() - 1) : i;
                if (cnt++ == colors.size())
                {
                    i++;
                    cnt = 0;
                }
                htmlStr += colors.at(idx);
                htmlStr += (iter.second) + "</td></tr>";
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
