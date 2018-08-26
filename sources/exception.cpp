// DOT exception base class

#include <dot/exception.h>
#include <utility>
#include <string>
#include <sstream>

namespace dot
{
    class_name_type exception::class_name = "exception";
    class_name_type typecast_exception::class_name = "typecast_exception";

    class exception::instance
    {
    public:
        explicit instance(const char* message)
            : m_message(message), m_backtrace(trace::stack::thread_stack())
        {
        }

        const char* what() const { return m_message.c_str(); }
        const trace::stack& backtrace() const { return m_backtrace; }

    private:
        std::string m_message;
        trace::stack m_backtrace;
    };

    exception::exception(const char* message)
        : m_instance(new instance(message))
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

    class_name_type exception::who() const
    {
        return exception::class_name;
    }

    namespace
    {
        std::string generate_typecast_exception_message(class_name_type to_type, class_name_type from_type)
        {
            std::stringstream message_stream;
            message_stream << "Unable to cast type '" << from_type << "' to type '" << to_type << "'.";
            return message_stream.str();
        }
    }

    typecast_exception::typecast_exception(class_name_type to_type, class_name_type from_type)
        : base(generate_typecast_exception_message(to_type, from_type).c_str())
    {
    }

    class_name_type typecast_exception::who() const
    {
        return typecast_exception::class_name;
    }
}

// Unicode signature: Владимир Керимов
