#pragma once

#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class MemLeakController : public IController
{
    private:
        IModel& model_;

    public:
        MemLeakController(IModel& model) noexcept;
        ~MemLeakController() override = default;

        void requestProcess(const QString& str) override;
};
