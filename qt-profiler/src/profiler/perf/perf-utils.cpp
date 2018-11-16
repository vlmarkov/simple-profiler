#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-utils.hpp>


RingBuffer::RingBuffer(const int fd)
{
    if (fd < 0)
        throw Exception("Failed to mmap perf event, not valid fd");

    auto* mmap = ::mmap(NULL, RingBuffer::mmapSizeGet_(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (mmap == MAP_FAILED)
        throw Exception("Failed to RingBuffer::mmap()");

    this->mpage_    = static_cast<struct perf_event_mmap_page*>(mmap);
    this->prevHead_ = 0;
}

RingBuffer::~RingBuffer()
{
    ::munmap(static_cast<void*>(this->mpage_), RingBuffer::mmapSizeGet_());
    // Do not propagate exception
}

bool RingBuffer::hasData()
{
    if ((this->mpage_->data_head != 0) &&
        (this->mpage_->data_head != this->prevHead_))
    {
        this->prevHead_ = this->mpage_->data_head;
        return true;
    }

    return false;
}

RecordPage RingBuffer::pageGet()
{
    const auto bytes = sizeof(RecordPage);

    RecordPage recordPage;

    ::memcpy(&recordPage, reinterpret_cast<void*>(this->mpage_), bytes);

    return recordPage;
}

RecordSample RingBuffer::sampleGet()
{
    const uintptr_t base = reinterpret_cast<uintptr_t>(this->mpage_) + this->mpage_->data_offset;
    const auto bytes     = sizeof(RecordSample);
    const auto startIdx  = this->mpage_->data_head - bytes;
    const auto endIdx    = startIdx + bytes;

    RecordSample recordSample;

    if (endIdx <= this->mpage_->data_size)
    {
        ::memcpy(&recordSample, (reinterpret_cast<void*>(base + startIdx)), bytes);
    }
    else
    {
        // Is this possible at all?
        throw Exception("Failed to RingBuffer::sampleGet()");
    }

    return recordSample;
}

unsigned RingBuffer::mmapSizeGet_()
{
    return static_cast<unsigned>(((1U << 8) + 1) * ::sysconf(_SC_PAGESIZE));
}
