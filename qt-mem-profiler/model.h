#ifndef MODEL_H
#define MODEL_H

#include <QMap>
#include <QString>

#include "observer.h"
#include "mallocobject.h"


class Model : public Observable
{
    public:
        Model();
        ~Model();

        void runMemProfiler(const QString& str);
        QString getMemProfillerResult(void);

    private:
        QString result_;

        QString libFileName_;
        QString logFileName_;

        void collectMallocUsage_(const QString& elfName);
        void readMallocUsage_(QMap<QString, MallocObject>& leakMap);
        void leakToSourceCode_(const QMap<QString, MallocObject>& leakMap, const QString& elfName);
        void readSourceCode_(const QString& pathTo, const int line, QString& leakLog);
};

#endif // MODEL_H
