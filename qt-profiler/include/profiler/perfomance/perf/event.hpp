#pragma once

#include <string>
#include <vector>

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>


// This is a wrapper for a 'perf_event_open'
// See for more details http://man7.org/linux/man-pages/man2/perf_event_open.2.html
class PerfEvent
{
    public:
        explicit PerfEvent(struct perf_event_attr& pe, pid_t pid);
        explicit PerfEvent(struct perf_event_attr& pe, pid_t pid, std::vector<uint32_t>& events, std::vector<uint64_t>& ids);
        ~PerfEvent();

        void start();
        void stop();

        int getFd();

    private:
        int fd_;
        int isGrouping_;

        int open_(struct perf_event_attr& pe, pid_t pid, int cpu, int gFd, unsigned long flags);
        void close_();
};
