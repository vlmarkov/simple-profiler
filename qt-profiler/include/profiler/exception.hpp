#pragma once

#include <QString>
#include <exception>


class Exception : public std::exception
{       
    public:
        Exception(const QString& err, const int code = 0);
        ~Exception() = default;

        virtual const char* what() const noexcept override;
        int code() const noexcept;

    private:
        int     code_;
        QString err_;
};
