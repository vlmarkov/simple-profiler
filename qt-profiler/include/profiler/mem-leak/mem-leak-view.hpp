#pragma once

#include <include/base-profiler.hpp>
#include <include/profiler/result.hpp>
#include <include/profiler/i-view.hpp>
#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class MemLeakView : public IView
{
    private:
        IModel&       model_;
        IController&  controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result res) noexcept;

    public:
        MemLeakView(IModel& model, IController& controller, BaseProfiler& baseProf);
        ~MemLeakView() override = default;

        void update(const IObserverEvent& event) override;
};
