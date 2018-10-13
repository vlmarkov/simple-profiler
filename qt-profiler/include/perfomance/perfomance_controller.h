#pragma once

#include "include/model.h"
#include "include/controller.h"


class PerfomanceController : public Controller
{
    public:
        PerfomanceController(Model& model) noexcept;
        ~PerfomanceController() override = default;

        void processRequest(const QString& str) override;

    private:
        Model& model_;
};
