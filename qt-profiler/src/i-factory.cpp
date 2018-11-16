#include <include/i-factory.hpp>
#include <include/profiler/exception.hpp>

#include <include/profiler/mem-leak/mem-leak-view.hpp>
#include <include/profiler/mem-leak/mem-leak-model.hpp>
#include <include/profiler/mem-leak/mem-leak-profiler.hpp>
#include <include/profiler/mem-leak/mem-leak-controller.hpp>

#include <include/profiler/perf/perf-controller.hpp>
#include <include/profiler/perf/perf-events-view.hpp>

#include <include/profiler/perf/perf-events-model.hpp>
#include <include/profiler/perf/perf-events-profiler.hpp>

#include <include/profiler/perf/perf-sampling-view.hpp>
#include <include/profiler/perf/perf-sampling-model.hpp>
#include <include/profiler/perf/perf-sampling-profiler.hpp>


auto IFactory::createProfiler(const IFactoryTypeProfiler& type) -> std::shared_ptr<IProfiler>
{
    switch (type)
    {
        case IFactoryTypeProfiler::mem_leak:
            return std::make_shared<MemLeakProfiler>();
        case IFactoryTypeProfiler::perf_event:
            return std::make_shared<PerfEventsProfiler>();
        case IFactoryTypeProfiler::perf_sample:
            return std::make_shared<PerfSamplingProfiler>();
    }
    throw Exception("Failed to create profiler");
}

auto IFactory::createModel(const IFactoryType& type) -> std::shared_ptr<IModel>
{
    QVector<uint32_t> hw_cpu = {
        PERF_COUNT_HW_CPU_CYCLES,
        PERF_COUNT_HW_INSTRUCTIONS
    };

    switch (type)
    {
        case IFactoryType::mem_leak:
            return std::make_shared<MemLeakModel>();
        case IFactoryType::perf_event:
            return std::make_shared<PerfEventsModel>(hw_cpu);
        case IFactoryType::perf_sample:
            return std::make_shared<PerfSamplingModel>();
    }

    throw Exception("Failed to create profiler model");
}

auto IFactory::createController(const IFactoryType&     type,
                                std::shared_ptr<IModel> model) -> std::shared_ptr<IController>
{
    switch (type)
    {
        case IFactoryType::mem_leak:
            return std::make_shared<MemLeakController>(*model.get());
        case IFactoryType::perf_event:
        case IFactoryType::perf_sample:
            return std::make_shared<PerfController>(*model.get());
    }

    throw Exception("Failed to create profiler controller");
}

auto IFactory::createView(const IFactoryType&          type,
                          std::shared_ptr<IModel>      model,
                          std::shared_ptr<IController> controller,
                          BaseProfiler&                baseProfiler) -> std::shared_ptr<IView>
{
    switch (type)
    {
        case IFactoryType::mem_leak:
            return std::make_shared<MemLeakView>(*model.get(), *controller.get(), baseProfiler);
        case IFactoryType::perf_event:
            return std::make_shared<PerfEventsView>(*model.get(), *controller.get(), baseProfiler);
        case IFactoryType::perf_sample:
            return std::make_shared<PerfSamplingView>(*model.get(), *controller.get(), baseProfiler);
    }

    throw Exception("Failed to create profiler");
}
