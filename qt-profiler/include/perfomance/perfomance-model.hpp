#pragma once

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>

#include <include/i-model.hpp>
#include <include/memory/malloc-object.hpp>


class PerfomanceModel : public IModel
{
    private:
        Result result_;

    public:
        PerfomanceModel() noexcept = default;
        ~PerfomanceModel() override = default;

        void requestProcess(const QString& request) override;
        Result getResult() noexcept override;
};
