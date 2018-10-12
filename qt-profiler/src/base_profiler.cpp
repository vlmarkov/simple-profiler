#include "include/base_profiler.h"
#include "include/memory_view.h"
#include "include/memory_model.h"
#include "include/memory_controller.h"

#include <QDebug>

#include <memory>


BaseProfiler::~BaseProfiler()
{
    ;
}

void BaseProfiler::memoryCheck(const QString& request)
{
    auto model      = std::make_unique<MemoryModel>();
    auto controller = std::make_unique<MemoryController>(*model.get());
    auto view       = std::make_unique<MemoryView>(*model.get(), *controller.get(), *this);

    controller->processRequest(request);

    // View instance will emit a signal
}

void BaseProfiler::perfomanceCheck(const QString& request)
{
    // Comming soon!
/*
    auto model      = std::make_unique<PerfomanceModel>();
    auto controller = std::make_unique<PerfomanceController>(*model.get());
    auto view       = std::make_unique<PerfomanceView>(*model.get(), *controller.get(), *this);

    controller->processRequest(request)
*/
    // View instance will emit a signal
}

QString BaseProfiler::getResult() noexcept
{
    return result_;
}

void BaseProfiler::setResult(const QString str) noexcept
{
    result_ = str;
    emit resultChanged();
}
