#pragma once

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>

#include <include/profiler/i-model.hpp>
#include <include/profiler/memory/malloc-object.hpp>


class MemoryModel : public IModel
{
    private:
        Result result_;

        const QString fileLib_;
        const QString fileLog_;

        void run_(const QString& request);
        void read_(QMap<QString, MallocObject>& map);
        void leakToSourceCode_(const QMap<QString, MallocObject>& map, const QString& elf);
        void readSourceCode_(const QString& path, const int ln, Result& res);

    public:
        MemoryModel() noexcept;
        ~MemoryModel() override = default;

        void requestProcess(const QString& request) override;
        Result getResult() noexcept override;
};
