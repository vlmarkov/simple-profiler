#include <memory>

#include <include/profiler/memory/memory-view.hpp>
#include <include/profiler/memory/memory-model.hpp>
#include <include/profiler/memory/memory-factory.hpp>
#include <include/profiler/memory/memory-profiler.hpp>
#include <include/profiler/memory/memory-controller.hpp>


void MemoryProfiler::run(const QString& request, BaseProfiler& baseProf)
{
    std::unique_ptr<IModel>      model(std::make_unique<MemoryModel>());
    std::unique_ptr<IController> controller(std::make_unique<MemoryController>(*model.get()));
    std::unique_ptr<IView>       view(std::make_unique<MemoryView>(*model.get(), *controller.get(), baseProf));

    controller->requestProcess(request);

    // View instance will emit a signal
}
