#pragma once

#include <QVector>

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
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
        explicit PerfEvent(struct perf_event_attr& pe, pid_t pid, QVector<uint32_t>& events, QVector<uint64_t>& ids);
        ~PerfEvent();

        void start();
        void stop();

        long getFd();

    private:
        long fd_;
        bool isGrouping_;

        long open_(struct perf_event_attr& pe, pid_t pid, int cpu, int gFd, unsigned long flags);
        void close_();
};
