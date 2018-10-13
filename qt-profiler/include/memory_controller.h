#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H

#include "model.h"
#include "controller.h"


class MemoryController : public Controller
{
    public:
        MemoryController(Model& model) noexcept;
        ~MemoryController() override = default;

        void processRequest(const QString& str) override;

    private:
        Model& model_;
};

#endif // MEMORY_CONTROLLER_H
