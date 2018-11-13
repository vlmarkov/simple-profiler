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
    auto event = Event::fail;

    try
    {
        boost::process::child child(pathToFile.toStdString());

        auto perfEvent  = PerfEvent(this->pe_, child.id());
        auto ringBuffer = RingBuffer(perfEvent.getFd());

        perfEvent.start();

        while (child.running())
        {
            if (ringBuffer.hasData())
            {
                PerfModelSampling::sampleCopy_(ringBuffer.sampleGet());
            }
        }

        child.wait();

        this->result_.add(qMakePair(IViewType::source,
            QString("Samples gather: "+ QString::number(this->samplesCnt_))));

        this->result_.add(qMakePair(IViewType::source,
            QString("Child " + QString::number(child.id()) + " ended with code " + QString::number(child.exit_code()))));

        perfEvent.stop();

        PerfModelSampling::samplesToResult_();
        event = Event::succses;
    }
    catch (Exception& exception)
    {
        this->result_.add(qMakePair(IViewType::error , QString(exception.what())));
    }
    catch (std::exception& exception)
    {
        this->result_.add(qMakePair(IViewType::error , QString(exception.what())));
    }
    catch (...)
    {
        this->result_.add(qMakePair(IViewType::error , QString("Unknow error")));
    }

    Observable::notify(event);
}

Result PerfModelSampling::getResult() noexcept
{
    return result_;
}

void PerfModelSampling::sampleCopy_(const RecordSample& sample)
{
    if (sample.header.type != PERF_RECORD_SAMPLE)
        return;

    auto find = this->samplesMap_.find(sample.ip);
    if (find != this->samplesMap_.end())
        find->second++;
    else
        this->samplesMap_.insert(std::make_pair(sample.ip, 1.0));

    this->samplesCnt_++;
}

void PerfModelSampling::samplesToResult_()
{
    for (auto i : this->samplesMap_)
    {
        QString value;
        value += "0x";
        value += QString::number(i.first, 16);
        value += "\t";
        value += QString::number(static_cast<double>(i.second) / static_cast<double>(this->samplesCnt_) * 100.0);
        value += " %";
        this->result_.add(qMakePair(IViewType::source, value));
    }
}
