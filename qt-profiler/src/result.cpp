#include "include/result.h"


Result::Result()
{
    ;
}

Result::~Result()
{
    ;
}

QVector<QPair<ViewType, QString>> Result::getData()
{
    return dataVector_;
}

void Result::addData(const QPair<ViewType, QString>& dataPair)
{
    dataVector_.push_back(dataPair);
}

void Result::clearData()
{
    dataVector_.clear();
}
