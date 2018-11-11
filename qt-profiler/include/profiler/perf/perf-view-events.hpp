#pragma once

#include <include/base-profiler.hpp>
#include <include/profiler/result.hpp>
#include <include/profiler/i-view.hpp>
#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class PerfViewEvents : public IView
{
    private:
        IModel&       model_;
        IController&  controller_;
        BaseProfiler& baseProf_;

    public:
        PerfViewEvents(IModel& model, IController& controller, BaseProfiler& baseProf);
        ~PerfViewEvents() override = default;

        void update(const Event& event) override;
};
