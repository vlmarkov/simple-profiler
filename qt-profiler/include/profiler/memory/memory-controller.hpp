#pragma once

#include <include/profiler/i-model.hpp>
#include <include/profiler/i-controller.hpp>


class MemoryController : public IController
{
    private:
        IModel& model_;

    public:
        MemoryController(IModel& model) noexcept;
        ~MemoryController() override = default;

        void requestProcess(const QString& str) override;
};
