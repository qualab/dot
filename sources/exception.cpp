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
            : m_message(message), m_stack(trace::stack::thread_stack())
        {
            m_stack.push(message, file, line);
        }

        const std::string& get_message() const { return m_message; }
        const trace::stack& get_stack() const { return m_stack; }

    private:
        std::string m_message;
        trace::stack m_stack;
    };

    exception::exception(const char* message, const char* file, int line)
        : m_instance(new instance(message, file, line))
    {
    }

    exception::exception(const std::string& message, const char* file, int line)
        : m_instance(new instance(message.c_str(), file, line))
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

    const char* exception::what() const
    {
        return m_instance->get_message().c_str();
    }

    const std::string& exception::get_message() const
    {
        return m_instance->get_message();
    }

    const trace::stack& exception::get_stack() const
    {
        return m_instance->get_stack();
    }

    const class_id& exception::get_class_id() const
    {
        return is_class<exception>();
    }

    class_name_type exception::get_class_name() const
    {
        return exception::class_name;
    }
}

// Unicode signature: Владимир Керимов
