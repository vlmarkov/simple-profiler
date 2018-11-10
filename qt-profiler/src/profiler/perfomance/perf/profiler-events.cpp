#include <QDebug>

#include <memory>

#include <include/profiler/perfomance/perf/exception.hpp>
#include <include/profiler/perfomance/perf/perf-view-events.hpp>
#include <include/profiler/perfomance/perf/perf-controller.hpp>
#include <include/profiler/perfomance/perf/profiler-events.hpp>
#include <include/profiler/perfomance/perf/perf-model-events.hpp>


void PerfProfilerEvents::run(const QString& pathToFile, BaseProfiler& baseProfiler)
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

        controller->requestProcess(pathToFile);

        // View instance will emit a signal
    }
    catch (Exception& exception)
    {
        qDebug() << QString(exception.what()) << " : " << exception.code();
    }
    catch (std::exception& exception)
    {
        qDebug() << QString(exception.what());
    }
    catch (...)
    {
        qDebug() << QString("Caught unexpected exception");
    }
}


