// DOT trace mechanics to 

#pragma once

#include <dot/public.h>

namespace dot
{
    // mandatory trace namespace
    struct trace
    {
        class scope;
        class stack;
    };

    // push name/file/line into stack while alive and pop on destruction
    class DOT_PUBLIC trace::scope
    {
    public:
        scope(const char* name, const char* file, int line);
        virtual ~scope();
    };

    // trace stack of the scopes inside each other
    class DOT_PUBLIC trace::stack
    {
    public:
        stack();
        ~stack();

        void push(const char* name, const char* file, int line);
        void pop();

        const char* top_name() const;
        const char* top_file() const;
        int top_line() const;

        static stack& thread_stack();

    private:
        class entry;
        class instance;

        instance* m_instance;
    };
}

#define DOT_TRACE_CALL trace::scope call_scope(__FUNCTION__, __FILE__, __LINE__);

// Unicode signature: Владимир Керимов
