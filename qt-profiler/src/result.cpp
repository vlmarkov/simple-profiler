#include "include/result.h"


QVector<QPair<ViewType, QString>> Result::get() noexcept
{
    return results_;
}

void Result::add(const QPair<ViewType, QString>& rhs) noexcept
{
    results_.push_back(rhs);
}

void Result::clear() noexcept
{
    results_.clear();
}
