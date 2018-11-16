#pragma once

#include <map>

#include <QString>

#include <include/profiler/i-model.hpp>
#include <include/profiler/perf/perf-utils.hpp>
#include <include/profiler/perf/perf-event.hpp>


class PerfSamplingModel : public IModel
{
    private:
        struct perf_event_attr pe_;

        long samplesCnt_;

        std::map<uint64_t, double> samplesMap_;

        void samplesToResult_(const QString& pathToFile);

        void sampleCopy_(const RecordSample& sample);

        Result result_;

    public:
        explicit PerfSamplingModel(uint32_t samplePeriod = 100000);
        ~PerfSamplingModel() = default;

        void process(const QString& pathTo) override;
        Result getResult() noexcept override;
};
