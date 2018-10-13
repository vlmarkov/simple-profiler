#pragma once

#include <QVector>
#include <QString>

#include "include/view.h"


class Result
{
    public:
        Result() = default;
        ~Result() = default;

        QVector<QPair<ViewType, QString>> get() noexcept;
        void add(const QPair<ViewType, QString>& dataPair) noexcept;
        void clear() noexcept;

    private:
        QVector<QPair<ViewType, QString>> results_;
};
