// DOT exception base class

#include <dot/fail.h>
#include <utility>
#include <string>
#include <sstream>

namespace dot
{
    using std::exception;

    DOT_CLASS_ID(fail::error)
    DOT_CLASS_ID(fail::bad_typecast)
    DOT_CLASS_ID(fail::unreadable_data)
    DOT_CLASS_ID(fail::null_reference)
    DOT_CLASS_ID(fail::non_comparable)
    DOT_CLASS_ID(fail::non_orderable)

    template<> DOT_CLASS_ID(rope<fail::info>)
    template<> DOT_CLASS_ID(rope<fail::info>::cow)

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
                << backtrace.top_name() << " в "
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
        : base(message), exception(message)
    {
    }

    fail::error::error(const char* message, const trace::stack& backtrace) noexcept
        : base(message, backtrace), exception(message)
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

    fail::null_reference::null_reference(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::null_reference::label() const noexcept
    {
        return "NULL REFERENCE";
    }

    fail::unreadable_data::unreadable_data(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::unreadable_data::label() const noexcept
    {
        return "UNREADABLE DATA";
    }

    fail::non_comparable::non_comparable(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::non_comparable::label() const noexcept
    {
        return "NON COMPARABLE";
    }

    fail::non_orderable::non_orderable(const char* message) noexcept
        : base(message)
    {
    }

    const char* fail::non_orderable::label() const noexcept
    {
        return "NON ORDERABLE";
    }
}

// Здесь должен быть Unicode
