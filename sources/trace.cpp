// DOT public declaration

#include <dot/trace.h>
#include <utility>
#include <string>
#include <stack>

namespace dot
{
    class trace::stack::entry
    {
    public:
        entry(const char* name, const char* file, int line) noexcept
            : m_name(name), m_file(file), m_line(line) { }

        const char* get_name() const noexcept { return m_name.c_str(); }
        const char* get_file() const noexcept { return m_file.c_str(); }
        int get_line() const noexcept { return m_line; }

    private:
        std::string m_name;
        std::string m_file;
        int m_line;
    };

    class trace::stack::instance
    {
    public:
        void push(const char* name, const char* file, int line) noexcept;
        void pop() noexcept;

        const char* top_name() const noexcept;
        const char* top_file() const noexcept;
        int top_line() const noexcept;

        bool empty() const noexcept;

        void set_message(const char* message) noexcept;
        const std::string& get_message() const noexcept;

    private:
        static const entry global;
        std::stack<entry> m_stack;
        std::string m_message;

        const entry& top() const noexcept;
    };

    const trace::stack::entry trace::stack::instance::global("global", "", 0);

    const trace::stack::entry& trace::stack::instance::top() const noexcept
    {
        return m_stack.empty() ? global : m_stack.top();
    }

    void trace::stack::instance::push(const char* name, const char* file, int line) noexcept
    {
        m_stack.push(entry(name, file, line));
    }

    void trace::stack::instance::pop() noexcept
    {
        m_stack.pop();
    }

    const char* trace::stack::instance::top_name() const noexcept
    {
        return top().get_name();
    }

    const char* trace::stack::instance::top_file() const noexcept
    {
        return top().get_file();
    }

    int trace::stack::instance::top_line() const noexcept
    {
        return top().get_line();
    }

    bool trace::stack::instance::empty() const noexcept
    {
        return m_stack.empty();
    }

    void trace::stack::instance::set_message(const char* message) noexcept
    {
        m_message = message ? message : "";
    }

    const std::string& trace::stack::instance::get_message() const noexcept
    {
        return m_message;
    }

    trace::scope::scope(const char* name, const char* file, int line) noexcept
    {
        stack::thread_stack().push(name, file, line);
    }

    trace::scope::~scope() noexcept
    {
        stack::thread_stack().pop();
    }

    trace::stack::stack() noexcept
        : m_instance(new instance())
    {
    }

    trace::stack::~stack() noexcept
    {
        delete m_instance;
    }

    trace::stack::stack(const trace::stack& another) noexcept
        : m_instance(new instance(*another.m_instance))
    {
    }

    trace::stack& trace::stack::operator = (const trace::stack& another) noexcept
    {
        *m_instance = *another.m_instance;
        return *this;
    }

    trace::stack::stack(trace::stack&& temporary) noexcept
        : m_instance(new instance(std::move(*temporary.m_instance)))
    {
    }

    trace::stack& trace::stack::operator = (trace::stack&& temporary) noexcept
    {
        *m_instance = std::move(*temporary.m_instance);
        return *this;
    }

    trace::stack::stack(const trace::stack& another, const char* message) noexcept
        : m_instance(new instance(*another.m_instance))
    {
        set_message(message);
    }

    trace::stack::stack(trace::stack&& temporary, const char* message) noexcept
        : m_instance(new instance(std::move(*temporary.m_instance)))
    {
        set_message(message);
    }

    void trace::stack::push(const char* name, const char* file, int line) noexcept
    {
        m_instance->push(name, file, line);
    }

    void trace::stack::pop() noexcept
    {
        m_instance->pop();
    }

    const char* trace::stack::top_name() const noexcept
    {
        return m_instance->top_name();
    }

    const char* trace::stack::top_file() const noexcept
    {
        return m_instance->top_file();
    }

    int trace::stack::top_line() const noexcept
    {
        return m_instance->top_line();
    }

    bool trace::stack::empty() const noexcept
    {
        return m_instance->empty();
    }

    bool trace::stack::not_empty() const noexcept
    {
        return !m_instance->empty();
    }

    void trace::stack::set_message(const char* message) noexcept
    {
        m_instance->set_message(message);
    }

    const char* trace::stack::get_message() const noexcept
    {
        return m_instance->get_message().c_str();
    }

    bool trace::stack::has_message() const noexcept
    {
        return !m_instance->get_message().empty();
    }

    trace::stack& trace::stack::thread_stack() noexcept
    {
        static thread_local stack local_stack;
        return local_stack;
    }

    std::ostream& operator << (std::ostream& stream, const trace::stack& source)
    {
        if (source.has_message())
        {
            stream << source.get_message() << "\n";
        }
        if (source.not_empty())
        {
            for (trace::stack backtrace = source;
                              backtrace.not_empty();
                              backtrace.pop())
            {
                stream << " >>-> "
                    << backtrace.top_name() << " at "
                    << backtrace.top_file() << '('
                    << backtrace.top_line() << ")\n";
            }
        }
        return stream;
    }

    std::istream& operator >> (std::istream& stream, trace::stack& /*destination*/)
    {
        // TODO: read traced stack
        return stream;
    }
}

// Unicode signature: Владимир Керимов
