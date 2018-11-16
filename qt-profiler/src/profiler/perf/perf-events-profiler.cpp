#include <include/i-factory.hpp>
#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-events-profiler.hpp>


void PerfEventsProfiler::run(const QString& pathTo, BaseProfiler& baseProfiler)
{
    try
    {
        auto model      = IFactory::createModel(IFactoryType::perf_event);
        auto controller = IFactory::createController(IFactoryType::perf_event, model);
        auto view       = IFactory::createView(IFactoryType::perf_event, model, controller, baseProfiler);

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


