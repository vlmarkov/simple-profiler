#ifndef MEMORY_VIEW_H
#define MEMORY_VIEW_H

#include "view.h"
#include "model.h"
#include "result.h"
#include "controller.h"
#include "base_profiler.h"


class MemoryView : public View
{
    public:
        MemoryView(Model* model, Controller* controller, BaseProfiler* baseProf);
        ~MemoryView();

        void update(const Event& event);

    private:
        Model*        model_;
        Controller*   controller_;
        BaseProfiler* baseProf_;

        QString toHtml_(Result res);
};

#endif // MEMORY_VIEW_H
