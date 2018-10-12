#pragma once

#include "view.h"
#include "model.h"
#include "result.h"
#include "controller.h"
#include "base_profiler.h"


class MemoryView : public View
{
    public:
        MemoryView(Model& model, Controller& controller, BaseProfiler& baseProf);
        ~MemoryView() override;

        void update(const Event& event) override;

    private:
        Model&        model_;
        Controller&   controller_;
        BaseProfiler& baseProf_;

        QString toHtml_(Result res) noexcept;
};
