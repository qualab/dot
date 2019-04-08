// DOT test checks implementation

#include <dot/test.h>
#include <dot/object.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>
#include <deque>

namespace dot
{
    DOT_CLASS_ID(test::check_fail)
    DOT_CLASS_ID(test::suite_fail)
    DOT_CLASS_ID(test::run_fail)

    namespace
    {
        std::deque<test::suite*> run_suites;
        thread_local std::deque<std::unique_ptr<const test::check_fail>> test_fails;

        template <typename fail_type, typename... argument_types>
        void register_fail(const argument_types&... arguments)
        {
            test_fails.push_back(std::make_unique<const fail_type>(arguments...));
        }
    }

    class test::output::instance
    {
    public:
        std::ostream& stream()
        {
            return my_stream;
        }

        std::string& message()
        {
            my_message = my_stream.str();
            return my_message;
        }

    private:
        std::stringstream my_stream;
        std::string my_message;
    };

    void test::run() noexcept
    {
        std::cout << "Dynamic Object Typification test run...\n" << std::endl;
        size_t suite_number = run_suites.size();
        size_t suite_passed = 0;
        size_t suite_failed = 0;
        auto interruption_found = std::find_if(run_suites.begin(), run_suites.end(),
            [&](test::suite* test_suite) -> bool
            {
                bool interrupt = false;
                try
                {
                    std::cout << " -> Run test suite \"" << test_suite->name() << "\" ... ";
                    test_suite->run();
                }
                catch (test::run_fail&)
                {
                    interrupt = true;
                }
                catch (test::suite_fail&)
                {
                }
                catch (fail::error& unhandled)
                {
                    output out;
                    out.print("Interrupted by exception " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                        unhandled.label(), unhandled.what());
                    register_fail<test::suite_fail>(out.message(), unhandled.backtrace());
                }
                catch (std::exception& unhandled)
                {
                    output out;
                    out.print("Interrupted by exception: " DOT_TEST_OUTPUT_ANY,
                        unhandled.what());
                    register_fail<test::suite_fail>(out.message());
                }
                catch (...)
                {
                    register_fail<test::suite_fail>(
                        "Interrupred by non-standard exception.");
                }
                if (test_fails.empty())
                {
                    ++suite_passed;
                    std::cout << "OK" << std::endl;
                }
                else
                {
                    ++suite_failed;
                    std::cout << test_fails.back()->label() << std::endl;
                    std::for_each(test_fails.begin(), test_fails.end(),
                        [](const std::unique_ptr<const test::check_fail>& fail)
                        {
                            std::cout << *fail;
                        }
                    );
                    test_fails.clear();
                }
                return interrupt;
            }
        );
        std::cout << "\n -- Test run is " << (suite_failed ? "FAILED" : "OK");
        std::cout << " (Total: " << suite_number
            << "; Passed: " << suite_passed
            << "; Failed: " << suite_failed
            << ")" << std::endl;
    }

    test::suite::suite() noexcept
    {
        run_suites.push_back(this);
    }

    test::check_fail::check_fail(const char* message) noexcept
        : base(message)
    {
    }

    test::check_fail::check_fail(const char* message, const trace::stack& backtrace) noexcept
        : base(message, backtrace)
    {
    }

    const char* test::check_fail::label() const noexcept
    {
        return "CHECK FAIL";
    }

    void test::check_fail::handle()
    {
        register_fail<check_fail>(*this);
    }

    test::suite_fail::suite_fail(const char* message) noexcept
        : base(message)
    {
    }

    test::suite_fail::suite_fail(const char* message, const trace::stack& backtrace) noexcept
        : base(message, backtrace)
    {
    }

    const char* test::suite_fail::label() const noexcept
    {
        return "SUITE FAIL";
    }

    void test::suite_fail::handle()
    {
        register_fail<suite_fail>(*this);
        throw *this;
    }

    test::run_fail::run_fail(const char* message) noexcept
        : base(message)
    {
    }

    test::run_fail::run_fail(const char* message, const trace::stack& backtrace) noexcept
        : base(message, backtrace)
    {
    }

    const char* test::run_fail::label() const noexcept
    {
        return "RUN FAIL";
    }

    void test::run_fail::handle()
    {
        register_fail<run_fail>(*this);
        throw *this;
    }

    test::output::output()
        : my_instance(new instance())
    {
    }

    test::output::~output()
    {
        delete my_instance;
    }

    std::ostream& test::output::stream()
    {
        return my_instance->stream();
    }

    void test::output::print(const char* description)
    {
        my_instance->stream() << description;
    }

    const char* test::output::message()
    {
        return my_instance->message().c_str();
    }

    bool test::output::find_placement(const char* description, const char*& before_end, const char*& after_begin)
    {
        static const size_t placement_length = std::strlen(DOT_TEST_OUTPUT_ANY);
        const char* placement = std::strstr(description, DOT_TEST_OUTPUT_ANY);
        if (!placement)
            return false;
        before_end = placement;
        after_begin = placement + placement_length;
        return true;
    }

    void test::output::print_range(const char* range_begin, const char* range_end)
    {
        my_instance->stream() << std::string(range_begin, range_end);
    }
}

// Здесь должен быть Unicode
