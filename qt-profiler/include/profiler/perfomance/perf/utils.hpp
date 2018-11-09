#pragma once

#include <perf-event.hpp>


class RingBuffer
{
    public:
        explicit RingBuffer(const int fd);
        ~RingBuffer();

        bool hasData();

        struct RecordPage pageGet();
        struct RecordSample sampleGet();

    private:
        struct perf_event_mmap_page* mpage_;
        size_t prevHead_;

        unsigned mmapSizeGet_();
};

struct RecordPage
{
    struct perf_event_mmap_page mpage;
};

struct RecordSample
{
    struct perf_event_header header;
    uint64_t ip;
    uint32_t pid;
    uint32_t tid;
};

struct ReadFormat
{
    uint64_t nr;
    struct {
        uint64_t value;
        uint64_t id;
    } values[];
};
