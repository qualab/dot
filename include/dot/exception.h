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
        static const class_id& id();
        virtual const class_id& who() const override;

    private:
        class instance;
        instance* m_instance;
    };

    class DOT_PUBLIC typecast_exception : public exception
    {
    public:
        explicit typecast_exception(const char* to_type, const char* from_type);

        typedef exception base;
        static const class_id& id();
        virtual const class_id& who() const override;
    };
}

// Unicode signature: Владимир Керимов
