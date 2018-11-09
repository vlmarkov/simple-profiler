#pragma once

#include <QVector>
#include <QString>

#include <include/i-view.hpp>


class Result
{
    private:
        QVector<QPair<IViewType, QString>> results_;

    public:
        Result() = default;
        ~Result() = default;

        QVector<QPair<IViewType, QString>> get() noexcept;
        void add(const QPair<IViewType, QString>& dataPair) noexcept;
        void clear() noexcept;
};
