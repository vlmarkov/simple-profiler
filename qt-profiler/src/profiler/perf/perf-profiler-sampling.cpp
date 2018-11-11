#include <QDebug>

#include <memory>

#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-controller.hpp>
#include <include/profiler/perf/perf-view-sampling.hpp>
#include <include/profiler/perf/perf-model-sampling.hpp>
#include <include/profiler/perf/perf-profiler-sampling.hpp>


void PerfProfilerSampling::run(const QString& pathToFile, BaseProfiler& baseProfiler)
{
    try
    {
        std::unique_ptr<IModel>      model(std::make_unique<PerfModelSampling>());
        std::unique_ptr<IController> controller(std::make_unique<PerfController>(*model.get()));
        std::unique_ptr<IView>       view(std::make_unique<PerfViewSampling>(*model.get(), *controller.get(), baseProfiler));

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
