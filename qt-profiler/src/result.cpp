#include <include/result.hpp>


QVector<QPair<IViewType, QString>> Result::get() noexcept
{
    return results_;
}

void Result::add(const QPair<IViewType, QString>& rhs) noexcept
{
    results_.push_back(rhs);
}

void Result::clear() noexcept
{
    results_.clear();
}
