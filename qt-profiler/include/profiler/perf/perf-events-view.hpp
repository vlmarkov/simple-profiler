#pragma once

#include <include/base-profiler.hpp>
#include <include/profiler/result.hpp>
#include <include/profiler/i-view.hpp>
#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class PerfEventsView : public IView
{
    private:
        IModel&       model_;
        IController&  controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result result) noexcept;

    public:
        PerfEventsView(IModel& model, IController& controller, BaseProfiler& baseProf);
        ~PerfEventsView() override = default;

        void update(const IObserverEvent& event) override;
};
