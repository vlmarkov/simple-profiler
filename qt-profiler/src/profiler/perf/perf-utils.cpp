#include <QTextStream>

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
    try
    {
        ::munmap(static_cast<void*>(this->mpage_), RingBuffer::mmapSizeGet_());
    }
    catch (...)
    {
        ; // Do not propagate exception
    }
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

FileReaderHotSpot::FileReaderHotSpot(const QString& fileName)
{
    this->file_.setFileName(fileName);
    if (!this->file_.open(QIODevice::ReadOnly | QFile::Text))
        throw Exception("Can't open file: " + this->file_.fileName());
}

FileReaderHotSpot::~FileReaderHotSpot()
{
    try
    {
        this->file_.close();
    }
    catch (...)
    {
        ; // Do not propagate exception
    }
}

QString FileReaderHotSpot::read()
{
    const QRegExp regExp("[ ]");
    const QRegExp regExpFile("[/]");

    QTextStream in(&this->file_);

    auto readString  = in.readAll();
    auto stringList  = readString.split(regExpFile, QString::SkipEmptyParts);
    auto srcFile     = stringList.value(stringList.length() - 1);
    auto srcFileList = srcFile.split(regExp, QString::SkipEmptyParts);

    return srcFileList.value(0);
}
