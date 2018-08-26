// DOT test checks implementation

#include <dot/test.h>
#include <dot/object.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <deque>

namespace dot
{
    class_name_type test::check_fail::class_name = "FAIL";
    class_name_type test::suite_fail::class_name = "ERROR";
    class_name_type test::run_fail::class_name = "FATAL ERROR";

    namespace
    {
        std::deque<test::suite*> run_suites;
        thread_local std::deque<test::check_fail> test_fails;
    }

    class test::output::instance
    {
    public:
        template <typename value_type>
        const char* format(const char* description, const value_type& value)
        {
            for (const char* place = strchr(description, format_begin);
                             place; 
                             place = strchr(place + 1, format_begin))
            {
                if (place[1] == format_any)
                {
                    m_stream << std::string(description, place) << value;
                    return place + 2;
                }
            }
            m_stream << description;
            return nullptr;
        }

        const char* print(const char* description)
        {
            m_stream << description;
            return nullptr;
        }

        const char* message()
        {
            m_message = m_stream.str();
            return m_message.c_str();
        }

    private:
        std::stringstream m_stream;
        std::string m_message;
    };

    void test::run()
    {
        std::cout << "Dynamic Object Typification test run..." << std::endl;
        size_t suite_number = run_suites.size();
        size_t suite_passed = 0;
        size_t suite_failed = 0;
        auto interruption_found = std::find_if(run_suites.begin(), run_suites.end(),
            [&](test::suite* test_suite) -> bool
            {
                bool interrupt = false;
                try
                {
                    std::cout << "\n -> Run test suite \"" << test_suite->name() << "\" ... ";
                    test_suite->run();
                }
                catch (test::run_fail&)
                {
                    interrupt = true;
                }
                catch (test::suite_fail&)
                {
                }
                catch (dot::exception& unhandled)
                {
                    output out;
                    out.print("Test suite interruption by exception %$: %$.",
                        unhandled.who(), unhandled.what());
                    test_fails.push_back(test::suite_fail(out.message()));
                }
                catch (std::exception& unhandled)
                {
                    output out;
                    out.print("Test suite interruption by exception: %$.", unhandled.what());
                    test_fails.push_back(test::suite_fail(out.message()));
                }
                catch (...)
                {
                    test_fails.push_back(test::suite_fail(
                        "Test suite interrupred by non-standard exception."));
                }
                if (test_fails.empty())
                {
                    ++suite_passed;
                    std::cout << "OK" << std::endl;
                }
                else
                {
                    ++suite_failed;
                    std::cout << test_fails.back().who() << std::endl;
                    std::for_each(test_fails.begin(), test_fails.end(),
                        [](const test::check_fail& fail)
                        {
                            std::cout << fail.who() << ": " << fail.what() << std::endl;
                            for (trace::stack backtrace = fail.backtrace();
                                              backtrace.not_empty();
                                              backtrace.pop())
                            {
                                std::cout << " > "
                                    << backtrace.top_name() << " at "
                                    << backtrace.top_file() << '('
                                    << backtrace.top_line() << ')'
                                    << std::endl;
                            }
                        }
                    );
                    test_fails.clear();
                }
                return interrupt;
            }
        );
        std::cout << "Tests: " << (suite_failed ? "FAIL" : "OK");
        std::cout << " (Total: " << suite_number
            << "; Passed: " << suite_passed
            << "; Failed: " << suite_failed
            << ")" << std::endl;
    }

    test::suite::suite()
    {
        run_suites.push_back(this);
    }

    test::check_fail::check_fail(const char* message)
        : base(message)
    {
    }

    void test::check_fail::handle()
    {
        test_fails.push_back(*this);
    }

    class_name_type test::check_fail::who() const
    {
        return check_fail::class_name;
    }

    test::suite_fail::suite_fail(const char* message)
        : base(message)
    {
    }

    void test::suite_fail::handle()
    {
        base::handle();
        throw *this;
    }

    class_name_type test::suite_fail::who() const
    {
        return suite_fail::class_name;
    }

    test::run_fail::run_fail(const char* message)
        : base(message)
    {
    }

    class_name_type test::run_fail::who() const
    {
        return run_fail::class_name;
    }

    test::output::output()
        : m_instance(new instance())
    {
    }

    test::output::~output()
    {
        delete m_instance;
    }

    const char* test::output::print(const char* description)
    {
        return m_instance->print(description);
    }

    const char* test::output::message()
    {
        return m_instance->message();
    }

    template<> const char* test::output::format(const char* description, const object& value) { return m_instance->format(description, value); }

    template<> const char* test::output::format(const char* description, const int64& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const int32& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const int16& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const int8& value) { return m_instance->format(description, value); }

    template<> const char* test::output::format(const char* description, const uint64& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const uint32& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const uint16& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const uint8& value) { return m_instance->format(description, value); }

    template<> const char* test::output::format(const char* description, const double& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const float& value) { return m_instance->format(description, value); }

    template<> const char* test::output::format(const char* description, const bool& value) { return m_instance->format(description, value); }
    template<> const char* test::output::format(const char* description, const char& value) { return m_instance->format(description, value); }

    template<> const char* test::output::format(const char* description, const char* const& value) { return m_instance->format(description, value); }
}

// Unicode signature: Владимир Керимов
