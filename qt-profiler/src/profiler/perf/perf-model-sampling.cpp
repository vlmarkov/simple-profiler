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

void PerfModelSampling::process(const QString& pathTo)
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
                PerfModelSampling::sampleCopy_(ringBuffer.sampleGet());
            }
        }

        child.wait();

        perfEvent.stop();

        PerfModelSampling::samplesToResult_(pathTo);
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

#include <iostream>
#include <string>
#include <QFile>

void PerfModelSampling::samplesToResult_(const QString& pathToFile)
{
    const std::string addr2line("addr2line");
    const std::string cmd = addr2line + " -e " + pathToFile.toStdString() + "0x";

    std::map<double, uint64_t> tmpMap;

    for (auto i : this->samplesMap_)
        tmpMap.insert(std::make_pair((static_cast<double>(i.second) / static_cast<double>(this->samplesCnt_) * 100.0), i.first));

    for (auto i : tmpMap)
    {
        QString addr = QString("0x") + QString::number(i.second, 16);

        std::string cmd = "addr2line -e " + pathToFile.toStdString() + " " + addr.toStdString();

        boost::process::ipstream out;
        boost::process::system(cmd, boost::process::std_out > "addr2line.log");

        QFile f("addr2line.log");
        if (!f.open(QIODevice::ReadOnly | QFile::Text))
            std::cerr << "err" << std::endl;

        QTextStream in(&f);

        QString value;
        value += addr;
        value += "\t";
        value += QString::number(i.first);
        value += "% == ";
        value += in.readAll();
        this->result_.add(qMakePair(IViewType::source, value));
    }
}
