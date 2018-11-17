#include <include/i-factory.hpp>
#include <include/profiler/mem-leak/mem-leak-profiler.hpp>


void MemLeakProfiler::run(const QString& request, BaseProfiler& baseProf)
{
    auto model      = IFactory::createModel(IFactoryType::mem_leak);
    auto controller = IFactory::createController(IFactoryType::mem_leak, model);
    auto view       = IFactory::createView(IFactoryType::mem_leak, model, controller, baseProf);

    controller->process(request); // View instance will emit a signal
}
