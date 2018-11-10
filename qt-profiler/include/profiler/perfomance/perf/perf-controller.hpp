#pragma once

#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class PerfController : public IController
{
    private:
        IModel& model_;

    public:
        PerfController(IModel& model) noexcept;
        ~PerfController() override = default;

        void requestProcess(const QString& request) override;
};
