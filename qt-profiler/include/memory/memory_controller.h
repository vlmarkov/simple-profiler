#pragma once

#include "include/model.h"
#include "include/controller.h"


class MemoryController : public Controller
{
    public:
        MemoryController(Model& model) noexcept;
        ~MemoryController() override = default;

        void requestProcess(const QString& str) override;

    private:
        Model& model_;
};
