#pragma once

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>

#include <include/profiler/i-model.hpp>
#include <include/profiler/mem-leak/mem-leak-object.hpp>


class MemLeakModel : public IModel
{
    private:
        Result result_;

        const QString fileLib_;
        const QString fileLog_;

        void run_(const QString& request);
        void read_(QMap<QString, MemLeakObj>& map);
        void leakToSourceCode_(const QMap<QString, MemLeakObj>& map, const QString& elf);
        void readSourceCode_(const QString& path, const int ln, Result& res);

    public:
        MemLeakModel() noexcept;
        ~MemLeakModel() override = default;

        void process(const QString& request) override;
        Result getResult() noexcept override;
};
