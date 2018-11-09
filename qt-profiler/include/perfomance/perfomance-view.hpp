#pragma once

#include <include/i-view.hpp>
#include <include/i-model.hpp>
#include <include/i-controller.hpp>
#include <include/result.hpp>
#include <include/base-profiler.hpp>


class PerfomanceView : public IView
{
    private:
        IModel&        model_;
        IController&   controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result result) noexcept;

    public:
        PerfomanceView(IModel& model, IController& controller, BaseProfiler& baseProf);
        ~PerfomanceView() override = default;

        void update(const Event& event) override;
};
