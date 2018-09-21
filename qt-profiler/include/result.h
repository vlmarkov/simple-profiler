#ifndef RESULT_H
#define RESULT_H

#include <QVector>
#include <QString>

#include "view_type.h"


class Result
{
    public:
        Result();
        ~Result();

        QVector<QPair<ViewType, QString>> getData();
        void addData(const QPair<ViewType, QString>& dataPair);
        void clearData();

    private:
        QVector<QPair<ViewType, QString>> dataVector_;
};

#endif // RESULT_H
