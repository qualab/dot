// DOT exception hierarchy

#pragma once

#include <dot/trace.h>
#include <dot/copyable.h>
#include <exception>

namespace dot
{
    class DOT_PUBLIC fail
    {
    public:
        fail() = delete;

        class info;

        class error;
        class bad_typecast;
        class null_reference;
        class unreadable_data;
    };

    class DOT_PUBLIC fail::info
    {
    public:
        explicit info(const char* message) noexcept;
        explicit info(const char* message, const trace::stack& backtrace) noexcept;

        virtual const char* what() const noexcept;
        const trace::stack& backtrace() const noexcept;

   private:
        trace::stack m_backtrace;
    };

    DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const fail::info& source);
    DOT_PUBLIC std::istream& operator >> (std::istream& stream, fail::info& destination);

    class DOT_PUBLIC fail::error : public copyable<fail::info>, public std::exception
    {
    public:
        explicit error(const char* message) noexcept;
        explicit error(const char* message, const trace::stack& backtrace) noexcept;

        virtual const char* what() const noexcept override;
        virtual const trace::stack& backtrace() const noexcept;
        virtual const char* label() const noexcept;

        typedef copyable<fail::info> base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;
    };

    DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const fail::error& source);
    DOT_PUBLIC std::istream& operator >> (std::istream& stream, fail::error& destination);

    class DOT_PUBLIC fail::bad_typecast : public fail::error
    {
    public:
        explicit bad_typecast(const class_id& to_type, const class_id& from_type) noexcept;
        virtual const char* label() const noexcept override;

        typedef fail::error base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;
    };

    class DOT_PUBLIC fail::null_reference : public fail::error
    {
    public:
        explicit null_reference(const char* message) noexcept;
        virtual const char* label() const noexcept override;

        typedef fail::error base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;
    };

    class DOT_PUBLIC fail::unreadable_data : public fail::error
    {
    public:
        explicit unreadable_data(const char* message) noexcept;
        virtual const char* label() const noexcept override;

        typedef fail::error base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;
    };

    template<> DOT_PUBLIC const class_id& copyable<fail::info>::id() noexcept;

    template<> DOT_PUBLIC const class_id& copyable<fail::info>::data::id() noexcept;
}

// Unicode signature: Владимир Керимов
