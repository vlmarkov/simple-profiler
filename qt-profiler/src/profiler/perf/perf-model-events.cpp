#include <QDebug>

#include <boost/process.hpp>

#include <include/profiler/exception.hpp>
#include <include/profiler/perf/perf-utils.hpp>
#include <include/profiler/perf/perf-model-events.hpp>


PerfModelEvents::PerfModelEvents(QVector<uint32_t>& hw) : hw_(hw)
{
    ::memset(&this->pe_, 0, sizeof(struct perf_event_attr));

    this->pe_.type           = PERF_TYPE_HARDWARE;
    this->pe_.size           = sizeof(struct perf_event_attr);
    this->pe_.config         = this->hw_[0];
    this->pe_.disabled       = 1;
    this->pe_.exclude_kernel = 1;
    this->pe_.exclude_hv     = 1;
    this->pe_.read_format    = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

    this->hw_id_  = QVector<uint64_t>(this->hw_.size());
    this->hw_val_ = QVector<uint64_t>(this->hw_.size());
}

void PerfModelEvents::process(const QString& pathTo)
{
    try
    {
        boost::process::child child(pathTo.toStdString());

        auto perfEvent = PerfEvent(this->pe_, child.id(), this->hw_, this->hw_id_);

        perfEvent.start();

        child.wait();

        qDebug() << "Child " << child.id() << " ended with code " << child.exit_code();

        perfEvent.stop();

        {
            // TODO: move to view module
            char buf[4096] = { 0 };
            struct ReadFormat *rf = (struct ReadFormat *)buf;

            ::read(perfEvent.getFd(), buf, sizeof(buf));

            for (size_t i = 0; i < rf->nr; i++)
            {
                for (size_t j = 0; j < rf->nr; j++)
                {
                   if (rf->values[i].id == this->hw_id_[j])
                    {
                        this->hw_val_[j] = rf->values[i].value;
                        break;
                    }
                }
            }

            qDebug() << "PERF_COUNT_HW_CPU_CYCLES   : " << (this->hw_val_[0] ? this->hw_val_[0] : -1);
            qDebug() << "PERF_COUNT_HW_INSTRUCTIONS : " << (this->hw_val_[1] ? this->hw_val_[1] : -1);
        }
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

Result PerfModelEvents::getResult() noexcept
{
    return result_;
}
