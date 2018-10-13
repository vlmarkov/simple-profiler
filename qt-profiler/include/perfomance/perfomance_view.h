#pragma once

#include "include/view.h"
#include "include/model.h"
#include "include/result.h"
#include "include/controller.h"
#include "include/base_profiler.h"


class PerfomanceView : public View
{
    public:
        PerfomanceView(Model& model, Controller& controller, BaseProfiler& baseProf);
        ~PerfomanceView() override = default;

        void update(const Event& event) override;

    private:
        Model&        model_;
        Controller&   controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result result) noexcept;
};
