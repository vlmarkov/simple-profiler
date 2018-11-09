#pragma once

#include <vector>
#include <string>

#include <include/i-profiler.hpp>
#include <include/perfomance/perf/event.hpp>


class PerfProfilerEvents : public IProfiler
{
    public:
        explicit PerfProfilerEvents();
        explicit PerfProfilerEvents(std::vector<uint32_t>& hw);
        ~PerfProfilerEvents() = default;

        void run(std::string& args) override;

    private:
        std::vector<uint32_t> hw_;     // Group hw request
        std::vector<uint64_t> hw_id_;  // Group hw request-id
        std::vector<uint64_t> hw_val_; // Group hw request-value

        struct perf_event_attr pe_;
};
