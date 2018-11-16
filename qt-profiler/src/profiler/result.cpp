#include <include/profiler/result.hpp>


QVector<QPair<IViewType, QString>> Result::get() noexcept
{
    return results_;
}

void Result::add(const QPair<IViewType, QString>& rhs)
{
    results_.push_back(rhs);
}

void Result::clear()
{
    results_.clear();
}
