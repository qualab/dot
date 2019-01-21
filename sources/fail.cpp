// DOT exception base class

#include <dot/fail.h>
#include <utility>
#include <string>
#include <sstream>

namespace dot
{
    fail::info::info(const char* message) noexcept
        : my_backtrace(trace::stack::thread_stack(), message)
    {
    }

    fail::info::info(const char* message, const trace::stack& backtrace) noexcept
        : my_backtrace(backtrace, message)
    {
    }

    const char* fail::info::what() const noexcept
    {
        return my_backtrace.get_message();
    }

    const trace::stack& fail::info::backtrace() const noexcept
    {
        return my_backtrace;
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
        : base(message), std::exception(message)
    {
    }

    fail::error::error(const char* message, const trace::stack& backtrace) noexcept
        : base(message, backtrace), std::exception(message)
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

    const char* fail::error::label() const noexcept
    {
        return "ERROR";
    }

    const class_id& fail::error::id() noexcept
    {
        static const class_id fail_error_id("fail::error");
        return fail_error_id;
    }

    std::ostream& operator << (std::ostream& stream, const fail::error& source)
    {
        stream << " !!>> " << source.label() << ": " << *source;
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

    const char* fail::bad_typecast::label() const noexcept
    {
        return "BAD TYPECAST";
    }

    const class_id& fail::bad_typecast::id() noexcept
    {
        static const class_id fail_bad_typecast_id("fail::bad_typecast");
        return fail_bad_typecast_id;
    }

    fail::null_reference::null_reference(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::null_reference::label() const noexcept
    {
        return "NULL REFERENCE";
    }

    const class_id& fail::null_reference::id() noexcept
    {
        static class_id fail_null_reference_id("fail::null_reference");
        return fail_null_reference_id;
    }

    fail::unreadable_data::unreadable_data(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::unreadable_data::label() const noexcept
    {
        return "UNREADABLE DATA";
    }

    const class_id& fail::unreadable_data::id() noexcept
    {
        static class_id fail_unreadable_data_id("fail::unreadable_data");
        return fail_unreadable_data_id;
    }

    fail::non_comparable::non_comparable(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::non_comparable::label() const noexcept
    {
        return "NON COMPARABLE";
    }

    const class_id& fail::non_comparable::id() noexcept
    {
        static class_id fail_non_comparable_id("fail::non_comparable");
        return fail_non_comparable_id;
    }

    fail::non_orderable::non_orderable(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::non_orderable::label() const noexcept
    {
        return "NON ORDERABLE";
    }

    const class_id& fail::non_orderable::id() noexcept
    {
        static class_id fail_non_orderable_id("fail::non_orderable");
        return fail_non_orderable_id;
    }

    template<> const class_id& copyable_of<fail::info>::id() noexcept
    {
        static class_id copyable_fail_info_id("copyable<fail::info>");
        return copyable_fail_info_id;
    }

    template<> const class_id& copyable_of<fail::info>::data::id() noexcept
    {
        static class_id copyable_fail_info_data_id("copyable<fail::info>::data");
        return copyable_fail_info_data_id;
    }
}

// Здесь должен быть Unicode
