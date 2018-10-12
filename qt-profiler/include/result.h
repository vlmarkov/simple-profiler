#pragma once

#include <QVector>
#include <QString>

#include "view_type.h"


class Result
{
    public:
        Result() = default;
        ~Result();

        QVector<QPair<ViewType, QString>> get() noexcept;
        void add(const QPair<ViewType, QString>& dataPair) noexcept;
        void clear() noexcept;

    private:
        QVector<QPair<ViewType, QString>> dataVector_;
};
