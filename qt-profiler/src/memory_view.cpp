#include "include/model.h"
#include "include/controller.h"
#include "include/memory_view.h"

#include <QDebug>


MemoryView::MemoryView(Model&        model,
                       Controller&   controller,
                       BaseProfiler& baseProf):
    model_(model),
    controller_(controller),
    baseProf_(baseProf)
{
    model_.Observable::add(this);
    controller_.Observable::add(this);
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
            case ViewType::source:
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

            case ViewType::leak:
            {
                htmlStr += "<tr><td style='color:red'>";
                htmlStr += (i.second) + "      ::POSSIBLE MEMORY LEAK::";
                htmlStr += "</td></tr>";

                break;
            }

            case ViewType::line:
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
