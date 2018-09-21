#include "include/base_profiler.h"
#include "include/memory_view.h"
#include "include/memory_model.h"
#include "include/memory_controller.h"

#include <QDebug>


BaseProfiler::BaseProfiler()
{
    ;
}

BaseProfiler::~BaseProfiler()
{
    ;
}

void BaseProfiler::analyze_memory(const QString& str)
{
    Model      *model      = new MemoryModel();
    Controller *controller = new MemoryController(model);
    View       *view       = new MemoryView(model, controller, this);

    controller->processRequest(str);

    // view will emit a signal

    delete model;
    delete controller;
    delete view;
}

void BaseProfiler::analyze_perfomance(const QString& str)
{
    // Comming soon!

    //model_      = new PerfomanceModel();
    //controller_ = new PerfomanceController(model);
    //view_       = new PerfomanceView(model, controller, this);

    //controller_->processRequest(str);

    // view will emit a signal

    //delete model;
    //delete controller;
    //delete view;
    return;
}

QString BaseProfiler::getResult()
{
    return result_;
}

void BaseProfiler::setResult(const QString str)
{
    result_ = str;
    emit resultChanged();
}
