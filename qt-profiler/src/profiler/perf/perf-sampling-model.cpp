#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

#include <boost/process.hpp>

#include <QTextStream>

#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-controller.hpp>
#include <include/profiler/perf/perf-sampling-model.hpp>


PerfSamplingModel::PerfSamplingModel(uint32_t samplePeriod)
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

void PerfSamplingModel::process(const QString& pathTo)
{
    auto event = IObserverEvent::fail;

    try
    {
        boost::process::child child(pathTo.toStdString());

        auto perfEvent  = PerfEvent(this->pe_, child.id());
        auto ringBuffer = RingBuffer(perfEvent.getFd());

        perfEvent.start();

        while (child.running())
        {
            if (ringBuffer.hasData())
            {
                this->sampleCopy_(ringBuffer.sampleGet());
            }
        }

        child.wait();

        perfEvent.stop();

        this->samplesToResult_(pathTo);
        event = IObserverEvent::succses;
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
        this->result_.add(qMakePair(IViewType::error , QString("Unknown error")));
    }

    Observable::notify(event);
}

Result PerfSamplingModel::getResult() noexcept
{
    return result_;
}

void PerfSamplingModel::sampleCopy_(const RecordSample& sample)
{
    if (sample.header.type != PERF_RECORD_SAMPLE)
        return;

    auto find = this->samplesMap_.find(sample.ip);
    if (find != this->samplesMap_.end())
    {
        find->second++;
    }
    else
    {
        this->samplesMap_.insert(std::make_pair(sample.ip, 1.0));
    }

    this->samplesCnt_++;
}

void PerfSamplingModel::samplesToResult_(const QString& pathToFile)
{
    const std::string cmd = "addr2line -e " + pathToFile.toStdString() + "0x";

    std::map<double, uint64_t> tmpMap;

    for (auto i : this->samplesMap_)
        tmpMap.insert(std::make_pair((static_cast<double>(i.second) / static_cast<double>(this->samplesCnt_) * 100.0), i.first));

    for (auto iter = tmpMap.rbegin(); iter != tmpMap.rend(); ++iter)
    {
        QString addr = QString("0x") + QString::number(iter->second, 16);

        std::string cmd = "addr2line -e " + pathToFile.toStdString() + " " + addr.toStdString();

        boost::process::ipstream out;
        boost::process::system(cmd, boost::process::std_out > "addr2line.log");

        FileReaderHotSpot fileHotSpot("addr2line.log");

        QString value = addr + " : " + QString::number(iter->first) + "%";

        this->result_.add(qMakePair(IViewType::source, fileHotSpot.read()));
        this->result_.add(qMakePair(IViewType::value, value));
    }

    remove("addr2line.log");
}
