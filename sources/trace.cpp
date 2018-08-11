// DOT public declaration

#pragma once

#include <dot/trace.h>
#include <string>
#include <stack>

namespace dot
{
    class trace::stack::entry
    {
    public:
        entry(const char* name, const char* file, int line)
            : m_name(name), m_file(file), m_line(line) { }

        const char* get_name() const { return m_name.c_str(); }
        const char* get_file() const { return m_file.c_str(); }
        int get_line() const { return m_line; }

    private:
        std::string m_name;
        std::string m_file;
        int m_line;
    };

    class trace::stack::instance
    {
    public:
        void push(const char* name, const char* file, int line);
        void pop();

        const char* top_name() const;
        const char* top_file() const;
        int top_line() const;

    private:
        static const entry global;
        std::stack<entry> m_stack;

        const entry& top() const;
    };

    const trace::stack::entry trace::stack::instance::global("global", "", 0);

    const trace::stack::entry& trace::stack::instance::top() const
    {
        return m_stack.empty() ? global : m_stack.top();
    }

    void trace::stack::instance::push(const char* name, const char* file, int line)
    {
        m_stack.push(entry(name, file, line));
    }

    void trace::stack::instance::pop()
    {
        m_stack.pop();
    }

    const char* trace::stack::instance::top_name() const
    {
        return top().get_name();
    }

    const char* trace::stack::instance::top_file() const
    {
        return top().get_file();
    }

    int trace::stack::instance::top_line() const
    {
        return top().get_line();
    }

    trace::scope::scope(const char* name, const char* file, int line)
    {
        stack::thread_stack().push(name, file, line);
    }

    trace::scope::~scope()
    {
        stack::thread_stack().pop();
    }

    trace::stack::stack()
        : m_instance(new instance())
    {
    }

    trace::stack::~stack()
    {
        delete m_instance;
    }

    void trace::stack::push(const char* name, const char* file, int line)
    {
        m_instance->push(name, file, line);
    }

    void trace::stack::pop()
    {
        m_instance->pop();
    }

    const char* trace::stack::top_name() const
    {
        return m_instance->top_name();
    }

    const char* trace::stack::top_file() const
    {
        return m_instance->top_file();
    }

    int trace::stack::top_line() const
    {
        return m_instance->top_line();
    }

    trace::stack& trace::stack::thread_stack()
    {
        static thread_local stack local_stack;
        return local_stack;
    }
}

// Unicode signature: Владимир Керимов
