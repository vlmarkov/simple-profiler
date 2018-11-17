#pragma once

#include <QString>

#include <include/profiler/i-model.hpp>


class MemLeakModel : public IModel
{
    private:
        Result        result_;

        const QString fileLib_;
        const QString fileLogLeak_;
        const QString fileLogAddr_;

    public:
        MemLeakModel() noexcept;
        ~MemLeakModel() override;

        void process(const QString& request) override;
        Result getResult() noexcept override;
};
