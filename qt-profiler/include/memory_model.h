#pragma once

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>

#include "model.h"
#include "malloc_object.h"


class MemoryModel : public Model
{
    public:
        MemoryModel() noexcept;
        ~MemoryModel() override;

        void processRequest(const QString& str) override;
        Result getResult() noexcept override;

    private:
        Result result_;

        const QString libFileName_;
        const QString logFileName_;

        void collectMallocUsage_(const QString& elf);
        void readMallocUsage_(QMap<QString, MallocObject>& map);
        void leakToSourceCode_(const QMap<QString, MallocObject>& map, const QString& elf);
        void readSourceCode_(const QString& path, const int ln, Result& res);
};
