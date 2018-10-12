#include "include/result.h"


Result::~Result()
{
    ;
}

QVector<QPair<ViewType, QString>> Result::get() noexcept
{
    return dataVector_;
}

void Result::add(const QPair<ViewType, QString>& rhs) noexcept
{
    dataVector_.push_back(rhs);
}

void Result::clear() noexcept
{
    dataVector_.clear();
}
