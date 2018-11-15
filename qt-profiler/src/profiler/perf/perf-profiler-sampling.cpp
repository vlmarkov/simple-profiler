#include <QDebug>

#include <memory>

#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-controller.hpp>
#include <include/profiler/perf/perf-view-sampling.hpp>
#include <include/profiler/perf/perf-model-sampling.hpp>
#include <include/profiler/perf/perf-profiler-sampling.hpp>


void PerfProfilerSampling::run(const QString& pathTo, BaseProfiler& baseProfiler)
{
    try
    {
        std::unique_ptr<IModel>      model(std::make_unique<PerfModelSampling>());
        std::unique_ptr<IController> controller(std::make_unique<PerfController>(*model.get()));
        std::unique_ptr<IView>       view(std::make_unique<PerfViewSampling>(*model.get(), *controller.get(), baseProfiler));

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
