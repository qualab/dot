// DOT test check declarations

#pragma once

#include <dot/exception.h>

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

        static void run();

        template <typename fail_type, typename argument_type>
        static check<fail_type, argument_type>
            make_check(const argument_type& argument,
                       const char* name,
                       const char* file,
                       int line)
        {
            return check<fail_type, argument_type>(argument, name, file, line);
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
        suite();
        virtual const char* name() const = 0;
        virtual void run() = 0;
    };

#define DOT_TEST_SUITE(suite_name) \
class test_suite_##suite_name : public test::suite \
{ \
public: \
    virtual const char* name() const override { return #suite_name; } \
    virtual void run() override { \
        trace::scope run_scope(#suite_name, __FILE__, __LINE__); \
        body(); \
    } \
private: \
    void body(); \
}; \
void test_suite_##suite_name::body()

    template <typename fail_type, typename argument_type>
    class test::check : public trace::scope
    {
    public:
        typedef trace::scope base;

        check(const argument_type& argument, const char* name, const char* file, int line);

        void is_true() const;
        void is_false() const;
        void is_null() const;
        void is_not_null() const;

        template <typename another_type> void is() const;
        template <typename another_type> void is_not() const;

        template <typename another_type> void operator == (const another_type& another) const;
        template <typename another_type> void operator != (const another_type& another) const;
        template <typename another_type> void operator <= (const another_type& another) const;
        template <typename another_type> void operator >= (const another_type& another) const;
        template <typename another_type> void operator < (const another_type& another) const;
        template <typename another_type> void operator > (const another_type& another) const;

        void no_exception() const;

        template <typename exception_type> void expect_exception() const;

    private:
        const argument_type& m_argument;
    };

#define DOT_TEST_CHECK(argument) test::make_check<test::check_fail>(argument, "check", __FILE__, __LINE__)
#define DOT_TEST_CHECK_NO_EXCEPTION(operation) DOT_TEST_CHECK([&]() { operation; }).no_exception()
#define DOT_TEST_CHECK_EXPECT_EXCEPTION(exception_class, operation) DOT_TEST_CHECK([&]() { operation; }).expect_exception<exception_class>()

#define DOT_TEST_ENSURE(argument) test::make_check<test::suite_fail>(argument, "ensure", __FILE__, __LINE__)
#define DOT_TEST_ENSURE_NO_EXCEPTION(operation) DOT_TEST_ENSURE([&]() { operation; }).no_exception()
#define DOT_TEST_ENSURE_EXPECT_EXCEPTION(exception_class, operation) DOT_TEST_ENSURE([&]() { operation; }).expect_exception<exception_class>()

#define DOT_TEST_ASSERT(argument) test::make_check<test::run_fail>(argument, "assert", __FILE__, __LINE__)
#define DOT_TEST_ASSERT_NO_EXCEPTION(operation) DOT_TEST_ASSERT([&]() { operation; }).no_exception()
#define DOT_TEST_ASSERT_EXPECT_EXCEPTION(exception_class, operation) DOT_TEST_ASSERT([&]() { operation; }).expect_exception<exception_class>()

    class DOT_PUBLIC test::check_fail : public dot::exception
    {
    public:
        check_fail(const char* message);
        virtual void handle();

        typedef dot::exception base;
        static class_name_type class_name;
        virtual class_name_type who() const override;
    };

    class DOT_PUBLIC test::suite_fail : public test::check_fail
    {
    public:
        suite_fail(const char* message);
        virtual void handle() override;

        typedef test::check_fail base;
        static class_name_type class_name;
        virtual class_name_type who() const override;
    };

    class DOT_PUBLIC test::run_fail : public test::suite_fail
    {
    public:
        run_fail(const char* message);

        typedef test::suite_fail base;
        static class_name_type class_name;
        virtual class_name_type who() const override;
    };

    class DOT_PUBLIC test::output
    {
    public:
        static const char format_begin = '%';
        static const char format_any = '$';

        class instance;

        output();
        ~output();

        template <typename value_type>
        const char* format(const char* description, const value_type& value)
        {
            static_assert(false, "Method template test::output::format must be specialized for this type.");
        }

        const char* print(const char* description);

        template <typename value_type, typename... argument_types>
        const char* print(
            const char* description,
            const value_type& value,
            const argument_types&... arguments)
        {
            const char* next = format(description, value);
            return next ? print(next, arguments...) : nullptr;
        }

        const char* message();

    private:
        instance* m_instance;

        output(const output& another) = delete;
        output& operator = (const output& another) = delete;

        output(output&& temporary) = delete;
        output& operator = (output&& temporary) = delete;
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
        catch (dot::exception& unhandled)
        {
            output out;
            out.print("Unhandled exception %$: %$.", unhandled.who(), unhandled.what());
            handle_fail<suite_fail>(out.message());
        }
        catch (std::exception& unhandled)
        {
            output out;
            out.print("Unhandled exception: %$.", unhandled.what());
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
    test::check<fail_type, argument_type>::check(const argument_type& argument, const char* name, const char* file, int line)
        : base(name, file, line), m_argument(argument)
    {
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_true() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return static_cast<bool>(m_argument);
            },
            "%$ is true",
            m_argument
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_false() const
    {
        test::ensure<fail_type>(
            [=]() -> bool {
                return !static_cast<bool>(m_argument);
            },
            "%$ is false",
            m_argument
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_null() const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const object&>(m_argument).is_null();
            },
            "%$ is null",
            m_argument
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::is_not_null() const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const object&>(m_argument).is_not_null();
            },
            "%$ is not null",
            m_argument
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::is() const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const object&>(m_argument).is<another_type>();
            },
            "%$ is %$",
            m_argument,
            another_type::class_name
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::is_not() const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return static_cast<const object&>(m_argument).is_not<another_type>();
            },
            "%$ is not %$",
            m_argument,
            another_type::class_name
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator == (const another_type& another) const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return m_argument == another;
            },
            "%$ == %$",
            m_argument,
            another
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator != (const another_type& another) const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return m_argument != another;
            },
            "%$ != %$",
            m_argument,
            another
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator <= (const another_type& another) const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return m_argument <= another;
            },
            "%$ <= %$",
            m_argument,
            another
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator >= (const another_type& another) const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return m_argument >= another;
            },
            "%$ >= %$",
            m_argument,
            another
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator < (const another_type& another) const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return m_argument < another;
            },
            "%$ < %$",
            m_argument,
            another
        );
    }

    template <typename fail_type, typename argument_type>
    template <typename another_type>
    void test::check<fail_type, argument_type>::operator > (const another_type& another) const
    {
        ensure<fail_type>(
            [=]() -> bool {
                return m_argument > another;
            },
            "%$ > %$",
            m_argument,
            another
        );
    }

    template <typename fail_type, typename argument_type>
    void test::check<fail_type, argument_type>::no_exception() const
    {
        try
        {
            m_argument();
        }
        catch (exception& unexpected)
        {
            test::output out;
            out.print("Unexpected exception %$: %$.", unexpected.who(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (std::exception& unexpected)
        {
            test::output out;
            out.print("Unexpected exception: %$.", unexpected.what());
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
            m_argument();
        }
        catch (exception_type&)
        {
            return;
        }
        catch (dot::exception& unexpected)
        {
            test::output out;
            out.print("Expected exception %$ but caught exception %$: %$.",
                exception_type::class_name, unexpected.who(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (std::exception& unexpected)
        {
            test::output out;
            out.print("Expected exception %$ but caught exception: %$.",
                exception_type::class_name, unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (...)
        {
            test::output out;
            out.print("Expected exception %$ but catch non-standard exception.",
                exception_type::class_name);
            handle_fail<fail_type>(out.message());
        }
        test::output out;
        out.print("Expected exception %$ but no exception was thrown.",
            exception_type::class_name);
        handle_fail<fail_type>(out.message());
    }

    class object;

    template<> DOT_PUBLIC const char* test::output::format(const char* description, const object& value);

    template<> DOT_PUBLIC const char* test::output::format(const char* description, const int64& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const int32& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const int16& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const int8& value);

    template<> DOT_PUBLIC const char* test::output::format(const char* description, const uint64& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const uint32& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const uint16& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const uint8& value);

    template<> DOT_PUBLIC const char* test::output::format(const char* description, const double& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const float& value);

    template<> DOT_PUBLIC const char* test::output::format(const char* description, const bool& value);
    template<> DOT_PUBLIC const char* test::output::format(const char* description, const char& value);

    template<> DOT_PUBLIC const char* test::output::format(const char* description, const char* const& value);
}

// Unicode signature: Владимир Керимов
