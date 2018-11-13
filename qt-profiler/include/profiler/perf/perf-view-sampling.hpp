#pragma once

#include <include/base-profiler.hpp>
#include <include/profiler/result.hpp>
#include <include/profiler/i-view.hpp>
#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class PerfViewSampling : public IView
{
    private:
        IModel&       model_;
        IController&  controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result res) noexcept;

    public:
        PerfViewSampling(IModel& model, IController& controller, BaseProfiler& baseProf);
        ~PerfViewSampling() override = default;

        void update(const Event& event) override;
};
