// DOT test check declarations

#pragma once

#include <dot/fail.h>
#include <iosfwd>

namespace dot
{
    class DOT_PUBLIC test
    {
    public:
        class suite;

        template <typename fail_type, typename argument_type>
        class check;

        class check_fail;
        class suite_fail;
        class run_fail;

        template <typename fail_type>
        class unhandled_exception;

        template <typename exception_type, typename fail_type>
        class exception_expected;

        class output;

        static void run() noexcept;

        template <typename fail_type, typename argument_type>
        static check<fail_type, argument_type>
            make_check(argument_type&& argument)
        {
            return check<fail_type, argument_type>(std::forward<argument_type>(argument));
        }

        template <typename fail_type, typename... argument_types>
        static void handle_fail(argument_types... arguments)
        {
            fail_type failure(arguments...);
            failure.handle();
        }

        template <typename fail_type, typename condition_type, typename... argument_types>
        static void ensure(
            const condition_type& condition,
            const char* description,
            const argument_types&... arguments);
    };

    class DOT_PUBLIC test::suite
    {
    public:
        suite() noexcept;
        virtual const char* name() const noexcept = 0;
        virtual void run() = 0;
    };

#define DOT_TEST_SUITE(suite_name) \
class test_suite_##suite_name : public test::suite \
{ \
public: \
    virtual const char* name() const noexcept override { return #suite_name; } \
    virtual void run() override { \
        trace::scope run_scope(#suite_name, __FILE__, __LINE__); \
        body(); \
    } \
private: \
    void body(); \
} g_##suite_name; \
void test_suite_##suite_name::body()

    template <typename fail_type, typename argument_type>
    class test::check
    {
    public:
        check(argument_type&& argument);

        void is_true() const;
        void is_false() const;
        void is_null() const;
        void is_not_null() const;

        template <typename another_type> void is() const;
        template <typename another_type> void is_not() const;

        template <typename another_type> void operator == (another_type&& another) const;
        template <typename another_type> void operator != (another_type&& another) const;
        template <typename another_type> void operator <= (another_type&& another) const;
        template <typename another_type> void operator >= (another_type&& another) const;
        template <typename another_type> void operator <  (another_type&& another) const;
        template <typename another_type> void operator >  (another_type&& another) const;

        void no_exception() const;

        template <typename exception_type> void expect_exception() const;

    private:
        argument_type&& my_argument;
    };

#define DOT_SCOPE(name) trace::scope(name, __FILE__, __LINE__)

#define DOT_CHECK(argument) DOT_SCOPE("check"), test::make_check<test::check_fail>(argument)
#define DOT_CHECK_NO_EXCEPTION(operation) DOT_CHECK([&]() { operation; }).no_exception()
#define DOT_CHECK_EXPECT_EXCEPTION(exception_class, operation) DOT_CHECK([&]() { operation; }).expect_exception<exception_class>()

#define DOT_ENSURE(argument) DOT_SCOPE("ensure"), test::make_check<test::suite_fail>(argument)
#define DOT_ENSURE_NO_EXCEPTION(operation) DOT_ENSURE([&]() { operation; }).no_exception()
#define DOT_ENSURE_EXPECT_EXCEPTION(exception_class, operation) DOT_ENSURE([&]() { operation; }).expect_exception<exception_class>()

#define DOT_ASSERT(argument) DOT_SCOPE("assert"), test::make_check<test::run_fail>(argument)
#define DOT_ASSERT_NO_EXCEPTION(operation) DOT_ASSERT([&]() { operation; }).no_exception()
#define DOT_ASSERT_EXPECT_EXCEPTION(exception_class, operation) DOT_ASSERT([&]() { operation; }).expect_exception<exception_class>()

    class DOT_PUBLIC test::check_fail : public fail::error
    {
    public:
        check_fail(const char* message) noexcept;
        check_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle();

        DOT_HIERARCHIC(fail::error);
    };

    class DOT_PUBLIC test::suite_fail : public test::check_fail
    {
    public:
        suite_fail(const char* message) noexcept;
        suite_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle() override;

        DOT_HIERARCHIC(test::check_fail);
    };

    class DOT_PUBLIC test::run_fail : public test::suite_fail
    {
    public:
        run_fail(const char* message) noexcept;
        run_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle() override;

        DOT_HIERARCHIC(test::suite_fail);
    };

#define DOT_TEST_OUTPUT_ANY "%$"

    class DOT_PUBLIC test::output
    {
    public:
        class instance;

        output();
        ~output();

        output(const output& another) = delete;
        output& operator = (const output& another) = delete;

        output(output&& temporary) = delete;
        output& operator = (output&& temporary) = delete;

        std::ostream& stream();

        template <typename value_type>
        const char* format(const char* description, const value_type& value)
        {
            const char *before_end, *after_begin;
            if (find_placement(description, before_end, after_begin))
            {
                print_range(description, before_end);
                stream() << value;
                return after_begin;
            }
            else
            {
                print(description);
                return nullptr;
            }
        }

        void print(const char* description);

        template <typename value_type, typename... argument_types>
        void print(
            const char* description,
            const value_type& value,
            const argument_types&... arguments)
        {
            if (const char* next = format(description, value))
                print(next, arguments...);
        }

        const char* message();

    private:
        instance* my_instance;

        bool find_placement(const char* description, const char*& before_end, const char*& after_begin);
        void print_range(const char* range_begin, const char* range_end);
    };

    template <typename fail_type, typename condition_type, typename... argument_types>
    void test::ensure(
        const condition_type& condition,
        const char* description,
        const argument_types&... arguments)
    {
        bool success = false;
        try
        {
            success = condition();
        }
        catch (fail::error& unhandled)
        {
            output out;
            out.print("Unhandled exception " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                unhandled.label(), unhandled.what());
            handle_fail<suite_fail>(out.message());
        }
        catch (std::exception& unhandled)
        {
            output out;
            out.print("Unhandled exception: " DOT_TEST_OUTPUT_ANY, unhandled.what());
            handle_fail<suite_fail>(out.message());
        }
        catch (...)
        {
            handle_fail<suite_fail>("Unhandled non-standard exception.");
        }
        if (!success)
        {
            output out;
            out.print(description, arguments...);
            handle_fail<fail_type>(out.message());
        }
    }

    template <typename fail_type, typename argument_type>
    test::check<fail_type, argument_type>::check(argument_type&& argument)
        : my_argument(std::forward<argument_type>(argument))
    {
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_true() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return static_cast<bool>(my_argument);
            },
            DOT_TEST_OUTPUT_ANY " is true",
            std::forward<argument_type>(my_argument)
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_false() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return !static_cast<bool>(my_argument);
            },
            DOT_TEST_OUTPUT_ANY " is false",
            std::forward<argument_type>(my_argument)
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_null() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const object&>(my_argument).is_null();
            },
            DOT_TEST_OUTPUT_ANY " is null",
            std::forward<argument_type>(my_argument)
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_not_null() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const object&>(my_argument).is_not_null();
            },
            DOT_TEST_OUTPUT_ANY " is not null",
            std::forward<argument_type>(my_argument)
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::is() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const hierarchic&>(my_argument).is<another_type>();
            },
            DOT_TEST_OUTPUT_ANY " is " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            another_type::id().name()
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::is_not() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const hierarchic&>(my_argument).is_not<another_type>();
            },
            DOT_TEST_OUTPUT_ANY " is not " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            another_type::id().name()
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator == (another_type&& another) const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return my_argument == another;
            },
            DOT_TEST_OUTPUT_ANY " == " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            std::forward<another_type>(another)
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator != (another_type&& another) const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return my_argument != another;
            },
            DOT_TEST_OUTPUT_ANY " != " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            std::forward<another_type>(another)
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator <= (another_type&& another) const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return my_argument <= another;
            },
            DOT_TEST_OUTPUT_ANY " <= " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            std::forward<another_type>(another)
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator >= (another_type&& another) const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return my_argument >= another;
            },
            DOT_TEST_OUTPUT_ANY " >= " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            std::forward<another_type>(another)
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator < (another_type&& another) const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return my_argument < another;
            },
            DOT_TEST_OUTPUT_ANY " < " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            std::forward<another_type>(another)
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator > (another_type&& another) const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return my_argument > another;
            },
            DOT_TEST_OUTPUT_ANY " > " DOT_TEST_OUTPUT_ANY,
            std::forward<argument_type>(my_argument),
            std::forward<another_type>(another)
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::no_exception() const
    {
        try
        {
            my_argument();
        }
        catch (fail::error& unexpected)
        {
            test::output out;
            out.print("Unexpected exception " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                unexpected.label(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (std::exception& unexpected)
        {
            test::output out;
            out.print("Unexpected exception: " DOT_TEST_OUTPUT_ANY, unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (...)
        {
            handle_fail<fail_type>("Unhandled non-standard exception.");
        }
    }

    template <typename fail_type, typename argument_type>
    template <typename exception_type>
    void test::check<fail_type, argument_type>::expect_exception() const
    {
        try
        {
            my_argument();
        }
        catch (exception_type&)
        {
            return;
        }
        catch (fail::error& unexpected)
        {
            test::output out;
            out.print("Expected exception " DOT_TEST_OUTPUT_ANY " but caught exception " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                exception_type::id().name(), unexpected.who().name(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (std::exception& unexpected)
        {
            test::output out;
            out.print("Expected exception " DOT_TEST_OUTPUT_ANY " but caught exception: " DOT_TEST_OUTPUT_ANY,
                exception_type::id().name(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (...)
        {
            test::output out;
            out.print("Expected exception " DOT_TEST_OUTPUT_ANY " but catch non-standard exception.",
                exception_type::id().name());
            handle_fail<fail_type>(out.message());
        }
        test::output out;
        out.print("Expected exception " DOT_TEST_OUTPUT_ANY " but no exception was thrown.",
            exception_type::id().name());
        handle_fail<fail_type>(out.message());
    }
}

// Unicode signature: Владимир Керимов
