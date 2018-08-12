// DOT exception base class

#pragma once

#include <dot/trace.h>
#include <dot/type.h>
#include <exception>

namespace dot
{
    class DOT_PUBLIC exception : public std::exception, public basement
    {
    public:
        explicit exception(const char* message, const char* file, int line);

        exception(const exception& another);
        exception(exception&& temporary);

        virtual ~exception();

        virtual const char* what() const noexcept override;
        virtual const trace::stack& backtrace() const;

        typedef basement base;
        static class_name_type class_name;
        virtual class_id&& get_class_id() const override;
        virtual class_name_type get_class_name() const override;

    private:
        class instance;
        instance* m_instance;
    };
}

// Unicode signature: Владимир Керимов
