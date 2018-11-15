#include <QDebug>

#include <memory>

#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-controller.hpp>
#include <include/profiler/perf/perf-view-events.hpp>
#include <include/profiler/perf/perf-model-events.hpp>
#include <include/profiler/perf/perf-profiler-events.hpp>


void PerfProfilerEvents::run(const QString& pathTo, BaseProfiler& baseProfiler)
{
    try
    {
        QVector<uint32_t> hw_cpu = {
            PERF_COUNT_HW_CPU_CYCLES,
            PERF_COUNT_HW_INSTRUCTIONS
        };

        std::unique_ptr<IModel>      model(std::make_unique<PerfModelEvents>(hw_cpu));
        std::unique_ptr<IController> controller(std::make_unique<PerfController>(*model.get()));
        std::unique_ptr<IView>       view(std::make_unique<PerfViewEvents>(*model.get(), *controller.get(), baseProfiler));

        controller->process(pathTo);

        // View instance will emit a signal
    }
    catch (Exception& exception)
    {
        baseProfiler.setResult(QString(exception.what())+ QString::number(exception.code()));
    }
    catch (std::exception& exception)
    {
        baseProfiler.setResult(QString(exception.what()));
    }
    catch (...)
    {
        baseProfiler.setResult(QString("Caught unexpected exception"));
    }
}


