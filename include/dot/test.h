// dot::test is minimal self-test module
// DOT_TEST_SUITE contains DOT_TEST_CHECK
// each check overloads operators ==, < etc.
// checks also have readable methods like
// not_null(), is_true(), is<type>() and so on
// test may be failed by rather level of fails:
// test::check_fail stores failure and output it
// at the end of suite run without interruption
// test::suite_fail interrupts suite run
// test::run_fail interrupts whole test run

#pragma once

#include <dot/fail.h>
#include <iosfwd>

namespace dot
{
    // test class used as name scope for test suite, test fails and so on
    class DOT_PUBLIC test
    {
    public:
        test() = delete;

        // test::suite used in DOT_TEST_SUITE(name) macro
        // whole test checks are divided to the such suites runs
        class suite;

        // test::check used in DOT_CHECK/DOT_ENSURE/DOT_ASSERT macros
        // all operators are overloaded for this class
        // it also contains useful methods like is<type>() or is_not_null()
        // any failed check generates proper failure as it is specified
        // DOT_CHECK macros instantiate test::check with test::check_fail
        // DOT_ENSURE macros instantiate test::check with test::suite_fail
        // DOT_ASSERT macros instantiate test::check with test::run_fail
        template <typename fail_type, typename argument_type>
        class check;

        // test::check_fail stores the failure and continue test suite run
        class check_fail;
        // test::suite_fail interrupts test suite run immediately
        class suite_fail;
        // test::run_fail interrupts whole test run immediately
        class run_fail;

        // special exception for any unhandled exception
        // which interrupts test suite run
        // may cause rather kinds of failures
        // suite_fail caused in case of unhandled exception
        // outside any exception expectation check
        template <typename fail_type>
        class unhandled_exception;

        // class for the exception expectation checks
        // used to expect specified type of exception
        // with the rather kinds of failures
        template <typename exception_type, typename fail_type>
        class exception_expected;

        // test::output used as abstraction from output specific
        // and text output formatted with arguments of any type
        class output;

        // test::run() start running of all registered test suites
        // with output of failures and their trace::stack information
        static void run() noexcept;

        // helpful method to specify test::check by argument type
        template <typename fail_type, typename argument_type>
        static check<fail_type, argument_type>
            make_check(argument_type&& argument)
        {
            return check<fail_type, argument_type>(std::forward<argument_type>(argument));
        }

        // helpful method to handle failure occured due test suite run
        template <typename fail_type, typename... argument_types>
        static void handle_fail(argument_types... arguments)
        {
            fail_type failure(arguments...);
            failure.handle();
        }

        // helpful method to ensure any test check has not failed
        // and describe failure in case when it occurs
        template <typename fail_type, typename condition_type, typename... argument_types>
        static void ensure(
            const condition_type& condition,
            const char* description,
            const argument_types&... arguments);
    };

    // test::suite is unit of test::run()
    // constructor of test::suite register it for test::run()
    // run() contains set of test checks to execute them
    // name() returns readable name which is related to the test checks
    // used in DOT_TEST_SUITE instead of direct usage as follows:
    // DOT_TEST_SUITE(some_test_suite_name)
    // {
    //     DOT_ENSURE(x).is_not_null();
    //     DOT_CHECK(x) == y;
    // }
    class DOT_PUBLIC test::suite
    {
    public:
        suite() noexcept;
        virtual const char* name() const noexcept = 0;
        virtual void run() = 0;
    };

// DOT_TEST_SUITE is used to simplify the writing of readable sets of the test checks
// use it as it shown in test::suite description
// it contain body() method which is implemented by 
// test checks in { } braces as it is simplest function body
// run() method is used to call from test::run() and store declatation position
// as __FILE__ and __LINE__ and suite name stored in trace::scope
// to show back trace stack in case of any failure due test suite execution
// macro also creates the global variable to register test suite via constructor
// last line begins the body declaration and waits for the body of body() method
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

    // test::check uses for any check in test run
    // stores universal reference to argument to check it
    // supports overloaded logical operators to be applied
    // to the stored argument to check is it equals or less
    // than some value of any other type
    // also supports helpful methods to check is that argument
    // null or not, is that true or is the instance of some type
    // test::check is instantiated with rather types of fails
    // it may cause single check failure without breaking the test pass
    // or may cause test suite or whole run filure with the run interruption
    // test::check is to be used as macro DOT_CHECK/DOT_ENSURE/DOT_ASSERT
    template <typename fail_type, typename argument_type>
    class test::check
    {
    public:
        // store universal reference to the argument
        check(argument_type&& argument);

        // is the argument true or false
        void is_true() const;
        void is_false() const;

        // is the argument null or not
        void is_null() const;
        void is_not_null() const;

        // is the argument instance of the type specified
        template <typename another_type> void is() const;
        template <typename another_type> void is_not() const;

        // overloaded operators are to compare the argument with any another value
        template <typename another_type> void operator == (another_type&& another) const;
        template <typename another_type> void operator != (another_type&& another) const;
        template <typename another_type> void operator <= (another_type&& another) const;
        template <typename another_type> void operator >= (another_type&& another) const;
        template <typename another_type> void operator <  (another_type&& another) const;
        template <typename another_type> void operator >  (another_type&& another) const;

        // method to check that no exception is expected
        // due the argument is being executed as function
        // it may generate failure of the type specified
        // instead of unhandled exception is not breaking the test pass
        // to be applied in exception dangerous place
        void no_exception() const;

        // method to check that exception occurs of the class specified
        // due the argument is being executed as function
        // it may generate failure of the type specified
        template <typename exception_type> void expect_exception() const;

    private:
        // universal reference to the argument passed
        argument_type&& my_argument;
    };

// creates test scope with the name specified
// useful for macro declaration to execute something in scope
// with store and trace the line of where it is declared
#define DOT_SCOPE(name) trace::scope(name, __FILE__, __LINE__)

// DOT_CHECK stores single check failure if it fails the check
// executing withing scope to trace file and line of declaration
// use it to check anything not breaking test pass for example as follows
// DOT_CHECK(x) == y;
// DOT_CHECK(z).is<object>();
#define DOT_CHECK(argument) DOT_SCOPE("check"), test::make_check<test::check_fail>(argument)

// DOT_CHECK_NO_EXCEPTION stores single check failure if any exception occurs
// due executing the operation specified as the argument of the macro
// operation is executing inside lambda which gets current scope as is
// is it to check the operation which potentially cause the exception as follows
// DOT_CHECK_NO_EXCEPTION(x = y);
// DOT_CHECK_NO_EXCEPTION(z == f());
#define DOT_CHECK_NO_EXCEPTION(operation) DOT_CHECK([&]() { operation; }).no_exception()

// DOT_CHECK_EXPECT_EXCEPTION stores single check failure in case
// when no exception occured or an exception occured of another class
// than the type is specified in first argument of this macro
// second argument is the operation which is being executed in lambda
// and executed within the scope of declaration
// use this macro to check correctness of the exception generation as follows
// DOT_CHECK_EXPECT_EXCEPTION(fail::error, z == f());
// DOT_CHECK_EXPECT_EXCEPTION(std::exception, x = y);
#define DOT_CHECK_EXPECT_EXCEPTION(exception_class, operation) DOT_CHECK([&]() { operation; }).expect_exception<exception_class>()

// DOT_ENSURE is same macros as DOT_CHECK but interrupts the test suite if it fails
#define DOT_ENSURE(argument) DOT_SCOPE("ensure"), test::make_check<test::suite_fail>(argument)
#define DOT_ENSURE_NO_EXCEPTION(operation) DOT_ENSURE([&]() { operation; }).no_exception()
#define DOT_ENSURE_EXPECT_EXCEPTION(exception_class, operation) DOT_ENSURE([&]() { operation; }).expect_exception<exception_class>()

// DOT_ASSERT is same macros as DOT_CHECK but interrupts whole test run if it fails
#define DOT_ASSERT(argument) DOT_SCOPE("assert"), test::make_check<test::run_fail>(argument)
#define DOT_ASSERT_NO_EXCEPTION(operation) DOT_ASSERT([&]() { operation; }).no_exception()
#define DOT_ASSERT_EXPECT_EXCEPTION(exception_class, operation) DOT_ASSERT([&]() { operation; }).expect_exception<exception_class>()

    // test::check_fail is base class for any test check failure
    // other types of failures are derived from this type
    // to handle it requires failure storage in test suite
    // with an information of test check failed shown at an end
    // of test suite execution which is most common case for any other
    // types of test check failures but the only which does not make interruption
    class DOT_PUBLIC test::check_fail : public fail::error
    {
    public:
        check_fail(const char* message) noexcept;
        check_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle();

        DOT_HIERARCHIC(fail::error);
    };

    // test::suite_fail is derived from test::check_fail
    // and same way handles with being stored in test suite
    // but also interrupts the execution of its test suite
    class DOT_PUBLIC test::suite_fail : public test::check_fail
    {
    public:
        suite_fail(const char* message) noexcept;
        suite_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle() override;

        DOT_HIERARCHIC(test::check_fail);
    };

    // test::run_fail is derived from test::check_fail
    // and same way handles with being stored in test suite
    // but also interrupts not only its test suite but whole test run
    class DOT_PUBLIC test::run_fail : public test::suite_fail
    {
    public:
        run_fail(const char* message) noexcept;
        run_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle() override;

        DOT_HIERARCHIC(test::suite_fail);
    };

// placeholder for insertion of argument any type in formatted text
// simple usage is "Some message with " DOT_TEST_OUTPUT_ANY " and so on"
#define DOT_TEST_OUTPUT_ANY "%$"

    // test::output is an abstraction layer from any way of output
    // also it provides the simplest way to print formatted message
    // with secure argument list pass by variadic template arguments
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

    // -- test methods implementation --

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

    // -- test::check methods implementation --

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
                exception_type::id().name(), unexpected.my_id().name(), unexpected.what());
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

// Здесь должен быть Unicode
