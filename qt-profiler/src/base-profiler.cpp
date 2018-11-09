#include <QDebug>

#include <memory>

#include <include/i-factory.hpp>
#include <include/i-profiler.hpp>
#include <include/base-profiler.hpp>
#include <include/profiler/memory/memory-factory.hpp>


void BaseProfiler::memoryCheck(const QString& request)
{
    std::shared_ptr<IFactory> factory = std::make_shared<MemoryFactory>();
    auto profiler = factory->createProfiler(IFactoryType::malloc);
    profiler->run(request, *this);
}

void BaseProfiler::perfomanceCheck(const QString& request)
{
    // Comming soon!
/*
    std::shared_ptr<IFactory> factory = std::make_shared<PerfomanceFactory>();
    auto profiler = factory->createProfiler(IFactory::perf); 
    profiler->run(request);
*/
    // run {
    //   std::unique_ptr<IModel>      model(std::make_unique<PerfomanceModel>());
    //   std::unique_ptr<IController> controller(std::make_unique<PerfomanceController>(*model.get()));
    //   std::unique_ptr<IView>       view(std::make_unique<PerfomanceView>(*model.get(), *controller.get(), *this));
    //   controller->requestProcess(request);
    // }

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
