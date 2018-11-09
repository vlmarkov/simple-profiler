#include <include/i-model.hpp>
#include <include/i-controller.hpp>
#include <include/memory/memory-view.hpp>


MemoryView::MemoryView(IModel&       model,
                       IController&  controller,
                       BaseProfiler& baseProf):
    model_(model),
    controller_(controller),
    baseProf_(baseProf)
{
    model_.Observable::add(*this);
    controller_.Observable::add(*this);
}

void MemoryView::update(const Event& event)
{
    switch (event)
    {
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

QString MemoryView::toHtml_(Result result) noexcept
{
    QString htmlStr;

    auto resultData = result.get();

    auto header = true;

    htmlStr += "<section><p></p><p></p>";

    for (auto i : resultData)
    {
        switch(i.first)
        {
            case IViewType::source:
            {
                if (!header)
                {
                    htmlStr += "</table>";
                    htmlStr += "<p></p>";
                }

                header = false;

                htmlStr += "<table border = 1 bgcolor=\"white\">";
                htmlStr += "<tr><th>";
                htmlStr += "**** " + (i.second) + " ****";
                htmlStr += "</th></tr>";

                htmlStr += "<tr>";

                break;
            }

            case IViewType::leak:
            {
                htmlStr += "<tr><td style='color:red'>";
                htmlStr += (i.second) + "      ::POSSIBLE MEMORY LEAK::";
                htmlStr += "</td></tr>";

                break;
            }

            case IViewType::line:
            {
                htmlStr += "<tr><td>" + (i.second) + "</td></tr>";
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
