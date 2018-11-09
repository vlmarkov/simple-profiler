#include <iostream>

#include <sys/wait.h>

#include <boost/process.hpp>

#include <include/perfomance/perf/utils.hpp>
#include <include/perfomance/perf/exception.hpp>
#include <include/perfomance/perf/profiler-events.hpp>


PerfProfilerEvents::PerfProfilerEvents()
{
    ::memset(&this->pe_, 0, sizeof(struct perf_event_attr));

    this->pe_.type           = PERF_TYPE_HARDWARE;
    this->pe_.size           = sizeof(struct perf_event_attr);
    this->pe_.config         = PERF_COUNT_HW_INSTRUCTIONS;
    this->pe_.disabled       = 1;
    this->pe_.exclude_kernel = 1;
    this->pe_.exclude_hv     = 1;
}

PerfProfilerEvents::PerfProfilerEvents(std::vector<uint32_t>& hw) : hw_(hw)
{
    ::memset(&this->pe_, 0, sizeof(struct perf_event_attr));

    this->pe_.type           = PERF_TYPE_HARDWARE;
    this->pe_.size           = sizeof(struct perf_event_attr);
    this->pe_.config         = this->hw_[0];
    this->pe_.disabled       = 1;
    this->pe_.exclude_kernel = 1;
    this->pe_.exclude_hv     = 1;
    this->pe_.read_format    = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

    this->hw_id_  = std::vector<uint64_t>(this->hw_.size());
    this->hw_val_ = std::vector<uint64_t>(this->hw_.size());
}

void PerfProfilerEvents::run(std::string& args)
{
    try
    {
        boost::process::child c(args);

        auto perfEvent = PerfEvent(this->pe_, c.id(), this->hw_, this->hw_id_);

        perfEvent.start();

        c.wait();

        std::cout << "Child " << c.id() << " ended with code " << c.exit_code() << std::endl;

        perfEvent.stop();

        {
            // TODO: move to view module
            char buf[4096] = { 0 };
            struct ReadFormat *rf = (struct ReadFormat *)buf;

            ::read(perfEvent.getFd(), buf, sizeof(buf));

            for (size_t i = 0; i < rf->nr; i++)
            {
                for (size_t j = 0; j < rf->nr; j++)
                {
                   if (rf->values[i].id == this->hw_id_[j])
                    {
                        this->hw_val_[j] = rf->values[i].value;
                        break;
                    }
                }
            }

            std::cout << "PERF_COUNT_HW_CPU_CYCLES   : " << (this->hw_val_[0] ? this->hw_val_[0] : -1) << std::endl;
            std::cout << "PERF_COUNT_HW_INSTRUCTIONS : " << (this->hw_val_[1] ? this->hw_val_[1] : -1) << std::endl;
        }
    }
    catch (Exception& exception)
    {
        std::cerr << exception.what() << " : " << exception.code() << std::endl;
    }
    catch (std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Caught unexpected exception" << std::endl;
    }
}
