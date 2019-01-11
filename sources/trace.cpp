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
            : my_name(name), my_file(file), my_line(line) { }

        const char* get_name() const noexcept { return my_name.c_str(); }
        const char* get_file() const noexcept { return my_file.c_str(); }
        int get_line() const noexcept { return my_line; }

    private:
        std::string my_name;
        std::string my_file;
        int my_line;
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
        std::stack<entry> my_stack;
        std::string my_message;

        const entry& top() const noexcept;
    };

    const trace::stack::entry trace::stack::instance::global("global", "", 0);

    const trace::stack::entry& trace::stack::instance::top() const noexcept
    {
        return my_stack.empty() ? global : my_stack.top();
    }

    void trace::stack::instance::push(const char* name, const char* file, int line) noexcept
    {
        my_stack.push(entry(name, file, line));
    }

    void trace::stack::instance::pop() noexcept
    {
        my_stack.pop();
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
        return my_stack.empty();
    }

    void trace::stack::instance::set_message(const char* message) noexcept
    {
        my_message = message ? message : "";
    }

    const std::string& trace::stack::instance::get_message() const noexcept
    {
        return my_message;
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
        : my_instance(new instance())
    {
    }

    trace::stack::~stack() noexcept
    {
        delete my_instance;
    }

    trace::stack::stack(const trace::stack& another) noexcept
        : my_instance(new instance(*another.my_instance))
    {
    }

    trace::stack& trace::stack::operator = (const trace::stack& another) noexcept
    {
        *my_instance = *another.my_instance;
        return *this;
    }

    trace::stack::stack(trace::stack&& temporary) noexcept
        : my_instance(new instance(std::move(*temporary.my_instance)))
    {
    }

    trace::stack& trace::stack::operator = (trace::stack&& temporary) noexcept
    {
        *my_instance = std::move(*temporary.my_instance);
        return *this;
    }

    trace::stack::stack(const trace::stack& another, const char* message) noexcept
        : my_instance(new instance(*another.my_instance))
    {
        set_message(message);
    }

    trace::stack::stack(trace::stack&& temporary, const char* message) noexcept
        : my_instance(new instance(std::move(*temporary.my_instance)))
    {
        set_message(message);
    }

    void trace::stack::push(const char* name, const char* file, int line) noexcept
    {
        my_instance->push(name, file, line);
    }

    void trace::stack::pop() noexcept
    {
        my_instance->pop();
    }

    const char* trace::stack::top_name() const noexcept
    {
        return my_instance->top_name();
    }

    const char* trace::stack::top_file() const noexcept
    {
        return my_instance->top_file();
    }

    int trace::stack::top_line() const noexcept
    {
        return my_instance->top_line();
    }

    bool trace::stack::empty() const noexcept
    {
        return my_instance->empty();
    }

    bool trace::stack::not_empty() const noexcept
    {
        return !my_instance->empty();
    }

    void trace::stack::set_message(const char* message) noexcept
    {
        my_instance->set_message(message);
    }

    const char* trace::stack::get_message() const noexcept
    {
        return my_instance->get_message().c_str();
    }

    bool trace::stack::has_message() const noexcept
    {
        return !my_instance->get_message().empty();
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
