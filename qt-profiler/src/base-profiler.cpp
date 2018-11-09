#include <QDebug>

#include <memory>

#include <include/base-profiler.hpp>
#include <include/memory/memory-view.hpp>
#include <include/memory/memory-model.hpp>
#include <include/memory/memory-controller.hpp>
#include <include/perfomance/perfomance-view.hpp>
#include <include/perfomance/perfomance-model.hpp>
#include <include/perfomance/perfomance-controller.hpp>


void BaseProfiler::memoryCheck(const QString& request)
{
    // Unique pointers usage with polymorphic behaviour
    std::unique_ptr<IModel>      model(std::make_unique<MemoryModel>());
    std::unique_ptr<IController> controller(std::make_unique<MemoryController>(*model.get()));
    std::unique_ptr<IView>       view(std::make_unique<MemoryView>(*model.get(), *controller.get(), *this));

    controller->requestProcess(request);

    // View instance will emit a signal
}

void BaseProfiler::perfomanceCheck(const QString& request)
{
    // Comming soon!

    std::unique_ptr<IModel>      model(std::make_unique<PerfomanceModel>());
    std::unique_ptr<IController> controller(std::make_unique<PerfomanceController>(*model.get()));
    std::unique_ptr<IView>       view(std::make_unique<PerfomanceView>(*model.get(), *controller.get(), *this));

    controller->requestProcess(request);

    // View instance will emit a signal
}

QString BaseProfiler::getResult() noexcept
{
    return result_;
}

void BaseProfiler::setResult(const QString& rhs) noexcept
{
    result_ = rhs;
    emit resultChanged();
}
