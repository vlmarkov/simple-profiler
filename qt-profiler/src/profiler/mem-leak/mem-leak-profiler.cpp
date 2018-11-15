#include <memory>

#include <include/profiler/mem-leak/mem-leak-view.hpp>
#include <include/profiler/mem-leak/mem-leak-model.hpp>
#include <include/profiler/mem-leak/mem-leak-profiler.hpp>
#include <include/profiler/mem-leak/mem-leak-controller.hpp>


void MemLeakProfiler::run(const QString& request, BaseProfiler& baseProf)
{
    std::unique_ptr<IModel>      model(std::make_unique<MemLeakModel>());
    std::unique_ptr<IController> controller(std::make_unique<MemLeakController>(*model.get()));
    std::unique_ptr<IView>       view(std::make_unique<MemLeakView>(*model.get(), *controller.get(), baseProf));

    controller->process(request);

    // View instance will emit a signal
}
