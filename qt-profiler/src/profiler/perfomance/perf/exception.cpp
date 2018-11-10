#include <include/profiler/perfomance/perf/exception.hpp>


Exception::Exception(const QString& err, const int code) : code_(code), err_(err)
{
    ;
}

const char* Exception::what() const noexcept
{
    return err_.toUtf8().constData();
}

const int Exception::code() const noexcept
{
    return code_;
}
