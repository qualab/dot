// DOT exception base class

#include <dot/fail.h>
#include <utility>
#include <string>
#include <sstream>

namespace dot
{
    fail::info::info(const char* message) noexcept
        : m_backtrace(trace::stack::thread_stack())
    {
        m_backtrace.set_message(message);
    }

    const char* fail::info::what() const noexcept
    {
        return m_backtrace.get_message();
    }

    const trace::stack& fail::info::backtrace() const noexcept
    {
        return m_backtrace;
    }

    std::ostream& operator << (std::ostream& stream, const fail::info& source)
    {
        stream << source.what() << "\n";
        for (trace::stack backtrace = source.backtrace();
                          backtrace.not_empty();
                          backtrace.pop())
        {
            stream << " ! -> "
                << backtrace.top_name() << " at "
                << backtrace.top_file() << '('
                << backtrace.top_line() << ")\n";
        }
        return stream;
    }

    std::istream& operator >> (std::istream& stream, fail::info& /*destination*/)
    {
        // TODO: read any failure info with backtrace
        return stream;
    }

    fail::error::error(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::error::what() const noexcept
    {
        return (*this)->what();
    }

    const trace::stack& fail::error::backtrace() const noexcept
    {
        return (*this)->backtrace();
    }

    const class_id& fail::error::id() noexcept
    {
        static const class_id fail_error_id("fail::error");
        return fail_error_id;
    }

    const class_id& fail::error::who() const noexcept
    {
        return fail::error::id();
    }

    std::ostream& operator << (std::ostream& stream, const fail::error& source)
    {
        stream << " !!>> " << source.who().name() << ": " << *source;
        return stream;
    }

    std::istream& operator >> (std::istream& stream, fail::error& /*destination*/)
    {
        // TODO: read any error from stream with backtrace
        return stream;
    }

    namespace
    {
        std::string generate_typecast_exception_message(const char* to_type_name, const char* from_type_name)
        {
            std::stringstream message_stream;
            message_stream << "Unable to cast type '" << from_type_name << "' to type '" << to_type_name << "'.";
            return message_stream.str();
        }
    }

    fail::bad_typecast::bad_typecast(const class_id& to_type, const class_id& from_type) noexcept
        : base(generate_typecast_exception_message(to_type.name(), from_type.name()).c_str())
    {
    }

    const class_id& fail::bad_typecast::id() noexcept
    {
        static const class_id fail_bad_typecast_id("fail::bad_typecast");
        return fail_bad_typecast_id;
    }

    const class_id& fail::bad_typecast::who() const noexcept
    {
        return fail::bad_typecast::id();
    }

    fail::null_reference::null_reference(const char* message) noexcept
        : base(message)
    {
    }

    const class_id& fail::null_reference::id() noexcept
    {
        static class_id fail_null_reference_id("fail::null_reference");
        return fail_null_reference_id;
    }

    const class_id& fail::null_reference::who() const noexcept
    {
        return fail::null_reference::id();
    }

    template<> const class_id& copyable<fail::info>::id() noexcept
    {
        static class_id copyable_fail_info_id("copyable<fail::info>");
        return copyable_fail_info_id;
    }

    template<> const class_id& copyable<fail::info>::data::id() noexcept
    {
        static class_id copyable_fail_info_data_id("copyable<fail::info>::data");
        return copyable_fail_info_data_id;
    }
}

// Unicode signature: Владимир Керимов
