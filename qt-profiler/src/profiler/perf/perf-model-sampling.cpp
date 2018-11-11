#include <algorithm>
#include <functional>

#include <boost/process.hpp>

#include <sys/wait.h>

#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-controller.hpp>
#include <include/profiler/perf/perf-model-sampling.hpp>

#include <QDebug>


PerfModelSampling::PerfModelSampling(uint32_t samplePeriod)
{
    ::memset(&(this->pe_), 0, sizeof(struct perf_event_attr));

    this->pe_.type           = PERF_TYPE_SOFTWARE;
    this->pe_.config         = PERF_COUNT_SW_TASK_CLOCK;
    this->pe_.size           = sizeof(struct perf_event_attr);
    this->pe_.disabled       = 1;
    this->pe_.exclude_kernel = 1;
    this->pe_.exclude_hv     = 1;
    this->pe_.read_format    = PERF_FORMAT_ID;
    this->pe_.sample_period  = samplePeriod; // Time between samples
    this->pe_.sample_id_all  = 1;
    this->pe_.sample_type    = PERF_SAMPLE_IP | PERF_SAMPLE_TID;

    this->samplesCnt_ = 0;
}

void PerfModelSampling::requestProcess(const QString& pathToFile)
{
    try
    {
        boost::process::child c(pathToFile.toStdString());

        auto perfEvent  = PerfEvent(this->pe_, c.id());
        auto ringBuffer = RingBuffer(perfEvent.getFd());

        perfEvent.start();

        while (c.running())
        {
            if (ringBuffer.hasData())
            {
                //auto page   = ringBuffer.pageGet();
                auto sample = ringBuffer.sampleGet();

                //PerfModelSampling::pagePrint_(page);
                //PerfModelSampling::samplePrint_(sample);
                PerfModelSampling::sampleCopy_(sample);
            }
        }

        c.wait();

        qDebug() << "Samples gather: " << this->samplesCnt_;
        qDebug() << "Child " << c.id() << " ended with code " << c.exit_code();

        perfEvent.stop();

        PerfModelSampling::mapPrint_();
    }
    catch (Exception& exception)
    {
        qDebug() << QString(exception.what()) << " : " << exception.code();
    }
    catch (std::exception& exception)
    {
        qDebug() << QString(exception.what());
    }
    catch (...)
    {
        qDebug() << QString("Caught unexpected exception");
    }
}

Result PerfModelSampling::getResult() noexcept
{
    return result_;
}

// TODO: move to view module
void PerfModelSampling::pagePrint_(const RecordPage& page)
{
    qDebug() << "The first metadata mmap page:";
    qDebug() << "\tversion        : " << page.mpage.version;
    qDebug() << "\tcompat version : " << page.mpage.compat_version;
    qDebug() << "\tlock           : " << page.mpage.lock;
    qDebug() << "\tindex          : " << page.mpage.index;
    qDebug() << "\toffset         : " << page.mpage.offset;
    qDebug() << "\ttime enabled   : " << page.mpage.time_enabled;
    qDebug() << "\ttime running   : " << page.mpage.time_running;
    qDebug() << "\tdata head      : " << page.mpage.data_head;
    qDebug() << "\tdata tail      : " << page.mpage.data_tail;
    qDebug() << "\tdata offset    : " << page.mpage.data_offset;
    qDebug() << "\tdata size      : " << page.mpage.data_size;
    qDebug() << "";
}

// TODO: move to view module
void PerfModelSampling::samplePrint_(const RecordSample& sample)
{
    if (sample.header.type != PERF_RECORD_SAMPLE)
        return;

    qDebug() << "Sample's header:";
    qDebug() << "\ttype : PERF_RECORD_SAMPLE";
    qDebug() << "\tmisc : " << sample.header.misc;
    qDebug() << "\tsize : " << sample.header.size;
    qDebug() << "";
    qDebug() << "Sample's data:";
    qDebug() << "\ttid  : "   << sample.tid;
    qDebug() << "\tpid  : "   << sample.pid;
    qDebug() << "";
    qDebug() << "\tip   : 0x" << sample.ip;
    qDebug() << "";
    qDebug() << "";
}

void PerfModelSampling::sampleCopy_(const RecordSample& sample)
{
    if (sample.header.type != PERF_RECORD_SAMPLE)
        return;

    // TODO: move to model module
    auto find = this->map_.find(sample.ip);
    if (find != this->map_.end())
        find->second++;
    else
        this->map_.insert(std::make_pair(sample.ip, 1.0));

    this->samplesCnt_++;
}

// TODO: move to view module
void PerfModelSampling::mapPrint_()
{
    qDebug() << "";
    qDebug() << "ip\t\t%";
    for (auto i : this->map_)
    {
        qDebug() << "0x" << i.first;
        qDebug() << "\t";
        qDebug() << i.second / (double)this->samplesCnt_ * 100.0;
        qDebug() << "";
    }
    qDebug() << "";
}
