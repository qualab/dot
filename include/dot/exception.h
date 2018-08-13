// DOT exception base class

#pragma once

#include <dot/trace.h>
#include <dot/type.h>
#include <exception>

namespace dot
{
    class DOT_PUBLIC exception : public std::exception, public hierarchic
    {
    public:
        explicit exception(const char* message);
        exception(const exception& another);
        exception(exception&& temporary);
        virtual ~exception();

        virtual const char* what() const noexcept override;
        virtual const trace::stack& backtrace() const;

        typedef hierarchic base;
        static class_name_type class_name;
        virtual class_name_type get_class_name() const override;

    private:
        class instance;
        instance* m_instance;
    };

    class DOT_PUBLIC typecast_exception : public exception
    {
    public:
        explicit typecast_exception(class_name_type to_type, class_name_type from_type);

        typedef exception base;
        static class_name_type class_name;
        virtual class_name_type get_class_name() const override;
    };
}

// Unicode signature: Владимир Керимов
