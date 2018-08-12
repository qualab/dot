// DOT exception base class

#pragma once

#include <dot/exception.h>
#include <utility>
#include <string>

namespace dot
{
    class_name_type exception::class_name = "exception";

    class exception::instance
    {
    public:
        instance(const char* message, const char* file, int line)
            : m_message(message), m_backtrace(trace::stack::thread_stack())
        {
            m_backtrace.push(message, file, line);
        }

        const char* what() const { return m_message.c_str(); }
        const trace::stack& backtrace() const { return m_backtrace; }

    private:
        std::string m_message;
        trace::stack m_backtrace;
    };

    exception::exception(const char* message, const char* file, int line)
        : m_instance(new instance(message, file, line))
    {
    }

    exception::exception(const exception& another)
        : m_instance(new instance(*another.m_instance))
    {
    }

    exception::exception(exception&& temporary)
        : m_instance(new instance(std::move(*temporary.m_instance)))
    {
    }

    exception::~exception()
    {
        delete m_instance;
    }

    const char* exception::what() const noexcept
    {
        return m_instance->what();
    }

    const trace::stack& exception::backtrace() const
    {
        return m_instance->backtrace();
    }

    class_id&& exception::get_class_id() const
    {
        return std::move(is_class<exception>());
    }

    class_name_type exception::get_class_name() const
    {
        return exception::class_name;
    }
}

// Unicode signature: Владимир Керимов
