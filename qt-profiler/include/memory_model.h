#ifndef MEMORY_MODEL_H
#define MEMORY_MODEL_H

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>

#include "model.h"
#include "malloc_object.h"


class MemoryModel : public Model
{
    public:
        MemoryModel();
        ~MemoryModel();

        void processRequest(const QString& str);
        Result getResult();

    private:
        Result result_;

        QString libFileName_;
        QString logFileName_;

        void collectMallocUsage_(const QString& elf);
        void readMallocUsage_(QMap<QString, MallocObject>& map);
        void leakToSourceCode_(const QMap<QString, MallocObject>& map, const QString& elf);
        void readSourceCode_(const QString& path, const int ln, Result& res);
};

#endif // MEMORY_MODEL_H
