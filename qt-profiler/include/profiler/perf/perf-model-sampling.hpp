#pragma once

#include <QString>

#include <map>

#include <include/profiler/i-model.hpp>
#include <include/profiler/perf/perf-utils.hpp>
#include <include/profiler/perf/perf-event.hpp>


class PerfModelSampling : public IModel
{
    private:
        struct perf_event_attr pe_;

        long samplesCnt_;

        std::map<uint64_t, double> map_;

        void mapPrint_();

        void pagePrint_(const RecordPage& page);
        void samplePrint_(const RecordSample& sample);
        void sampleCopy_(const RecordSample& sample);

        Result result_;

    public:
        explicit PerfModelSampling(uint32_t samplePeriod = 100000);
        ~PerfModelSampling() = default;

        void requestProcess(const QString& request) override;
        Result getResult() noexcept override;
};
