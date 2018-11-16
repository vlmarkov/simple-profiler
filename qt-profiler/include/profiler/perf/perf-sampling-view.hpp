#pragma once

#include <include/base-profiler.hpp>
#include <include/profiler/result.hpp>
#include <include/profiler/i-view.hpp>
#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class PerfSamplingView : public IView
{
    private:
        IModel&       model_;
        IController&  controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result res) noexcept;

    public:
        PerfSamplingView(IModel& model, IController& controller, BaseProfiler& baseProf);
        ~PerfSamplingView() override = default;

        void update(const IObserverEvent& event) override;
};
