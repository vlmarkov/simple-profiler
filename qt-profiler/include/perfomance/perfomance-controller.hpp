#pragma once

#include <include/i-model.hpp>
#include <include/i-controller.hpp>


class PerfomanceController : public IController
{
    private:
        IModel& model_;

    public:
        PerfomanceController(IModel& model) noexcept;
        ~PerfomanceController() override = default;

        void requestProcess(const QString& str) override;
};
