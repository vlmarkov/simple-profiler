#pragma once

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>

#include "include/model.h"
#include "include/malloc_object.h"


class PerfomanceModel : public Model
{
    public:
        PerfomanceModel() noexcept = default;
        ~PerfomanceModel() override = default;

        void requestProcess(const QString& request) override;
        Result getResult() noexcept override;

    private:
        Result result_;
};
