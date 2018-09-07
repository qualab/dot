// DOT trace mechanics to 

#pragma once

#include <dot/public.h>
#include <dot/stdfwd.h>

namespace dot
{
    // mandatory trace namespace
    struct trace
    {
        trace() = delete;

        class scope;
        class stack;
    };

    // push name/file/line into stack while alive and pop on destruction
    class DOT_PUBLIC trace::scope
    {
    public:
        scope(const char* name, const char* file, int line) noexcept;
        virtual ~scope() noexcept;
    };

    // trace stack of the scopes inside each other
    class DOT_PUBLIC trace::stack
    {
    public:
        stack() noexcept;
        ~stack() noexcept;

        stack(const stack& another) noexcept;
        stack& operator = (const stack& another) noexcept;

        stack(stack&& temporary) noexcept;
        stack& operator = (stack&& temporary) noexcept;

        stack(const stack& another, const char* message) noexcept;
        stack(stack&& temporary, const char* message) noexcept;

        void push(const char* name, const char* file, int line) noexcept;
        void pop() noexcept;

        const char* top_name() const noexcept;
        const char* top_file() const noexcept;
        int top_line() const noexcept;

        bool empty() const noexcept;
        bool not_empty() const noexcept;

        void set_message(const char* message) noexcept;
        const char* get_message() const noexcept;
        bool has_message() const noexcept;

        static stack& thread_stack() noexcept;

    private:
        class entry;
        class instance;

        instance* m_instance;
    };

    DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const trace::stack& source);
    DOT_PUBLIC std::istream& operator >> (std::istream& stream, trace::stack& destination);
}

#define DOT_TRACE_CALL trace::scope call_scope(__FUNCTION__, __FILE__, __LINE__);

// Unicode signature: Владимир Керимов
