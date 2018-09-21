#include "include/model.h"
#include "include/controller.h"
#include "include/memory_view.h"

#include <QDebug>


MemoryView::MemoryView(Model* model, Controller* controller, BaseProfiler* baseProf) :
    model_(model),
    controller_(controller),
    baseProf_(baseProf)
{
    model_->addObserver(this);
    controller_->addObserver(this);
}

MemoryView::~MemoryView()
{
    ;
}

void MemoryView::update(const Event& event)
{
    switch (event)
    {
        case Event::fail:
        {
            baseProf_->setResult(toHtml_(model_->getResult()));
            break;
        }
        case Event::succses:
        {
            baseProf_->setResult(toHtml_(model_->getResult()));
            break;
        }
    }
}

QString MemoryView::toHtml_(Result res)
{
    QString htmlStr;

    auto result = res.getData();

    htmlStr += "<section>";
    htmlStr += "<p></p>";
    htmlStr += "<p></p>";

    auto first = true;

    for (auto i = result.begin(); i != result.end(); i++)
    {
        if (i->first == ViewType::source)
        {
            if (!first)
            {
                htmlStr += "</table>";
                htmlStr += "<p></p>";
            }

            first = false;


            htmlStr += "<table border = 1 bgcolor=\"white\">";
            htmlStr += "<tr>";
            htmlStr += "<th>";
            htmlStr += "**** " + ((i)->second) + " ****";
            htmlStr += "</th>";
            htmlStr += "</tr>";

            htmlStr += "<tr>";
        }
        if (i->first == ViewType::leak)
        {
            htmlStr += "<tr>";
            htmlStr += "<td style='color:red'>";
            htmlStr += (i->second) + "      ::POSSIBLE MEMORY LEAK::";
            htmlStr += "</td>";
            htmlStr += "</tr>";
        }
        if (i->first == ViewType::line)
        {
            htmlStr += "<tr>";
            htmlStr += "<td>";
            htmlStr += (i->second);
            htmlStr += "</td>";
            htmlStr += "</tr>";
        }
    }

    htmlStr += "</table>";

    htmlStr += "<p></p>";
    htmlStr += "</section>";

    return htmlStr;
}
