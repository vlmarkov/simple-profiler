#pragma once

#include <QVector>
#include <QString>

#include <include/profiler/i-model.hpp>
#include <include/profiler/perf/perf-utils.hpp>
#include <include/profiler/perf/perf-event.hpp>


class PerfModelEvents : public IModel
{
    private:
        struct perf_event_attr pe_;

        QVector<uint32_t> hw_;     // Group hw request
        QVector<uint64_t> hw_id_;  // Group hw request-id
        QVector<uint64_t> hw_val_; // Group hw request-value

        Result result_;

    public:
        explicit PerfModelEvents(QVector<uint32_t>& hw);
        ~PerfModelEvents() = default;

        void process(const QString& pathTo) override;
        Result getResult() noexcept override;
};
