#pragma once

#include <map>
#include <string>

#include <include/i-profiler.hpp>
#include <include/perfomance/perf/event.hpp>
#include <include/perfomance/perf/utils.hpp>


class PerfProfilerSampling : public IProfiler
{
    public:
        explicit PerfProfilerSampling(uint32_t samplePeriod = 100000);
        ~PerfProfilerSampling() = default;

        void run(std::string& args) override;

    private:
        struct perf_event_attr pe_;

        long samplesCnt_;

        std::map<uint64_t, double> map_;

        void mapPrint_();

        void pagePrint_(const RecordPage& page);
        void samplePrint_(const RecordSample& sample);
        void sampleCopy_(const RecordSample& sample);
};
