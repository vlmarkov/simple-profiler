#include <include/profiler/perf/perf-event.hpp>
#include <include/profiler/exception.hpp>


PerfEvent::PerfEvent(struct perf_event_attr& pe, pid_t pid)
{
    this->isGrouping_ = false;
    this->fd_         = this->open_(pe, pid, -1, -1, 0);
}

PerfEvent::PerfEvent(struct perf_event_attr& pe, pid_t pid, QVector<uint32_t>& events, QVector<uint64_t>& ids)
{
    this->isGrouping_ = true;

    for (auto i = 0; i < events.size(); i++)
    {
        pe.config = events[i];
        auto fd = this->open_(pe, pid, -1, (i == 0) ? -1 : this->fd_, 0);

        if (i == 0)
        {
            this->fd_ = fd;
        }

        if (::ioctl(fd, PERF_EVENT_IOC_ID, &ids[i]) == -1)
            throw Exception("Failed to perf event: " + QString(strerror(errno)), errno);
    }
}

PerfEvent::~PerfEvent()
{
    try
    {
        this->close_();
    }
    catch (...)
    {
        ; // Do nothing
    }
}

// Start counting events or record sampling
void PerfEvent::start()
{
    if (::ioctl(this->fd_, PERF_EVENT_IOC_RESET, this->isGrouping_ ? PERF_IOC_FLAG_GROUP : 0) == -1)
        throw Exception("Failed to perf event: " + QString(strerror(errno)), errno);

    if (::ioctl(this->fd_, PERF_EVENT_IOC_ENABLE, this->isGrouping_ ? PERF_IOC_FLAG_GROUP : 0) == -1)
        throw Exception("Failed to perf event::start() " + QString(strerror(errno)), errno);
}

// Stop counting events or record sampling
void PerfEvent::stop()
{
    if (::ioctl(this->fd_, PERF_EVENT_IOC_DISABLE, this->isGrouping_ ? PERF_IOC_FLAG_GROUP : 0) == -1)
        throw Exception("Failed to stop(), " + QString(strerror(errno)), errno);
}

long PerfEvent::getFd()
{
    return this->fd_;
}

long PerfEvent::open_(struct perf_event_attr& pe, pid_t pid, int cpu, int gFd, unsigned long flags)
{
    // Glibc does not provide a wrapper for this system call
    auto fd = ::syscall(__NR_perf_event_open, &pe, pid, cpu, gFd, flags);
    if (fd < 0)
        throw Exception(QString("Failed to perf_event_open(), not valid fd"));

    return fd;
}

void PerfEvent::close_()
{
    auto rc = ::close(this->fd_);
    if (rc != 0)
        throw Exception(QString("Failed to close"), rc);
}
