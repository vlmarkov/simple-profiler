#pragma once

#include <memory>

#include <include/i-profiler.hpp>
#include <include/profiler/i-view.hpp>
#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


enum class IFactoryTypeProfiler { perf_event, perf_sample, mem_leak };

enum class IFactoryType { perf_event, perf_sample, mem_leak };


// This is a interface class
class IFactory
{
    public:
        static auto createProfiler(const IFactoryTypeProfiler& type)     -> std::shared_ptr<IProfiler>;

        static auto createModel(const IFactoryType& type)                -> std::shared_ptr<IModel>;

        static auto createController(const IFactoryType&     type,
                                     std::shared_ptr<IModel> model)      -> std::shared_ptr<IController>;

        static auto createView(const IFactoryType&          type,
                               std::shared_ptr<IModel>      model,
                               std::shared_ptr<IController> controller,
                               BaseProfiler&                baseProfiler) -> std::shared_ptr<IView>;
};
