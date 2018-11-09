#pragma once

#include <string>
#include <exception>


class Exception : public std::exception
{       
    public:
        Exception(const std::string& err, const int code = 0);
        ~Exception() = default;

        virtual const char* what() const noexcept override;
        const int code() const noexcept;

    private:
        int         code_;
        std::string err_;
};
