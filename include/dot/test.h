// Минимальный тестовый механизм для самотестирования библиотеки
// с удобной структурой наборов тестов в виде читаемых проверок 
// с бэктрейсами ошибок и понятными перегруженными операторами

#pragma once

#include <dot/fail.h>
#include <iosfwd>

namespace dot
{
    // тестовый класс используется как обязательный неймспейс
    class DOT_PUBLIC test
    {
    public:
        test() = delete;

        // набор тестов вида DOT_TEST_SUITE(name)
        class suite;

        // проверка вида DOT_CHECK/DOT_ENSURE/DOT_ASSERT
        // обычно ссылается на данные которые нужно проверить
        // для проверки использует перегруженные операторы
        template <typename fail_type, typename argument_type>
        class check;

        // ошибка при разовой проверке не прерывает другие тесты
        class check_fail;

        // ошибка при выполнении набора тестов прерывает текущий набор
        class suite_fail;

        // ошибка при выполнении тестирования прерывает весь запуск тестов
        class run_fail;

        // специальный шаблон для ошибки непредвиденного исключения
        // в случае необработанного исключения при выполнении набора
        // в зависимости от параметра шаблона генерирует разные ошибки
        template <typename fail_type>
        class unhandled_exception;

        // специальный шаблон для обработки исключения определённого класса
        // в случае отсутствия исключения или исключения другого класса
        // генерирует ошибку в зависимости от второго параметра шаблона
        template <typename exception_type, typename fail_type>
        class exception_expected;

        // абстракция для текстового вывода
        class output;

        // запуск всех написанных наборов тестов
        static void run() noexcept;

        // вспомогательный метод для читаемости создания проверок
        template <typename fail_type, typename argument_type>
        static check<fail_type, argument_type>
            make_check(argument_type&& argument)
        {
            return check<fail_type, argument_type>(std::forward<argument_type>(argument));
        }

        // вспомогательный метод для обработки ошибок с различными параметрами
        template <typename fail_type, typename... argument_types>
        static void handle_fail(argument_types... arguments)
        {
            fail_type failure(arguments...);
            failure.handle();
        }

        // вспомогательный метод для проверок критичных для всего набора тестов
        template <typename fail_type, typename condition_type, typename... argument_types>
        static void ensure(
            const condition_type& condition,
            const char* description,
            const argument_types&... arguments);
    };

    // набор тестов объявляется в виде макроса DOT_TEST_SUITE
    // с произвольным телом наподобие обычной функции
    class DOT_PUBLIC test::suite
    {
    public:
        suite() noexcept;
        virtual const char* name() const noexcept = 0;
        virtual void run() = 0;
    };

// макрос DOT_TEST_SUITE создаёт читаемый код метода набора тестовых проверок
// внутри можно описать обычное тело метода с DOT_CHECK/DOT_ENSURE/DOT_ASSERT
// dot::test::run() сам найдёт все описанные DOT_TEST_SUITE наборы тестов
// а выявленные ошибки получат читаемый бэктрейс вплоть до начала набора
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

    // любая проверка в наборе тестов это экземпляры test::check
    // неявно использующиеся в макросах DOT_CHECK/DOT_ENSURE/DOT_ASSERT
    // использует методы и перегруженные операторы либо исключения
    // test::check ссылается на левый операнд для читаемости проверок
    template <typename fail_type, typename argument_type>
    class test::check
    {
    public:
        // сохранение операнда операции проверки
        check(argument_type&& argument);

        // проверка на истинность
        void is_true() const;
        void is_false() const;

        // проверка на наличие данных
        void is_null() const;
        void is_not_null() const;

        // проверка на соответствие типа
        template <typename another_type> void is() const;
        template <typename another_type> void is_not() const;

        // сравнение с произвольными типами
        template <typename another_type> void operator == (another_type&& another) const;
        template <typename another_type> void operator != (another_type&& another) const;
        template <typename another_type> void operator <= (another_type&& another) const;
        template <typename another_type> void operator >= (another_type&& another) const;
        template <typename another_type> void operator <  (another_type&& another) const;
        template <typename another_type> void operator >  (another_type&& another) const;

        // проверка на отсутствие исключений в функции-операнде
        void no_exception() const;

        // проверка на исключение определённого класса в функции-операнде
        template <typename exception_type> void expect_exception() const;

    private:
        // ссылка на операнд для проверки
        argument_type&& my_argument;
    };

// создаёт по месту проверки метку дла бэктрейса
#define DOT_SCOPE(name) trace::scope(name, __FILE__, __LINE__)

// DOT_CHECK созаёт единичную проверку в теле набора тестов
// вида DOT_CHECK(x) == y; или DOT_CHECK(z).is<object>();
// содержится в специальном скоупе для получения бэктрейса
#define DOT_CHECK(argument) DOT_SCOPE("Проверка"), test::make_check<test::check_fail>(argument)

// DOT_CHECK_NO_EXCEPTION проверяет что единичная операция не вызывает исключений
// например DOT_CHECK_NO_EXCEPTION(x = y); или DOT_CHECK_NO_EXCEPTION(z == f());
#define DOT_CHECK_NO_EXCEPTION(operation) DOT_CHECK([&]() { operation; }).no_exception()

// DOT_CHECK_EXPECT_EXCEPTION проверяет что единичная операция указанная вторым аргументом
// вызывает при выполнении исключение указанного первым параметром макроса класса исключений
// пример: DOT_CHECK_EXPECT_EXCEPTION(fail::error, z == f());
// пример: DOT_CHECK_EXPECT_EXCEPTION(std::exception, x = y);
#define DOT_CHECK_EXPECT_EXCEPTION(exception_class, operation) DOT_CHECK([&]() { operation; }).expect_exception<exception_class>()

// DOT_ENSURE те же макросы что и для DOT_CHECK, но прерывают весь набор тестов
#define DOT_ENSURE(argument) DOT_SCOPE("Блокирующее условие"), test::make_check<test::suite_fail>(argument)
#define DOT_ENSURE_NO_EXCEPTION(operation) DOT_ENSURE([&]() { operation; }).no_exception()
#define DOT_ENSURE_EXPECT_EXCEPTION(exception_class, operation) DOT_ENSURE([&]() { operation; }).expect_exception<exception_class>()

// DOT_ASSERT те же макросы что и для DOT_CHECK, но прерывают весь запуск тестов
#define DOT_ASSERT(argument) DOT_SCOPE("Критическая проверка"), test::make_check<test::run_fail>(argument)
#define DOT_ASSERT_NO_EXCEPTION(operation) DOT_ASSERT([&]() { operation; }).no_exception()
#define DOT_ASSERT_EXPECT_EXCEPTION(exception_class, operation) DOT_ASSERT([&]() { operation; }).expect_exception<exception_class>()

    // test::check_fail генерирует исключение прерывающее только текущую проверку
    // с сообщением об ошибке и бэктрейсе с дальнейшим выполнением набора тестов
    class DOT_PUBLIC test::check_fail : public fail::error
    {
    public:
        check_fail(const char* message) noexcept;
        check_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle();

        DOT_HIERARCHIC(fail::error);
    };

    // test::suite_fail генерирует исключение прерывающее текущий набор тестов
    // с сообщением об ошибке и бэктрейсе как и в классе-предке test::check_fail
    class DOT_PUBLIC test::suite_fail : public test::check_fail
    {
    public:
        suite_fail(const char* message) noexcept;
        suite_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle() override;

        DOT_HIERARCHIC(test::check_fail);
    };

    // test::run_fail генерирует исключение прерывающее весь запуск тестов
    // с сообщением об ошибке и бэктрейсе как и в классе-предке test::suite_fail
    class DOT_PUBLIC test::run_fail : public test::suite_fail
    {
    public:
        run_fail(const char* message) noexcept;
        run_fail(const char* message, const trace::stack& backtrace) noexcept;
        virtual const char* label() const noexcept override;
        virtual void handle() override;

        DOT_HIERARCHIC(test::suite_fail);
    };

// простой плейсхолдер для вывода произвольных данных в сообщении
#define DOT_TEST_OUTPUT_ANY "%$"

    // test::output вспомогательный класс для абстракции вывода текста
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

    // -- шаблонные методы тестирования --

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
            out.print("Необработанная ошибка " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                unhandled.label(), unhandled.what());
            handle_fail<suite_fail>(out.message());
        }
        catch (std::exception& unhandled)
        {
            output out;
            out.print("Необработанное исключение: " DOT_TEST_OUTPUT_ANY, unhandled.what());
            handle_fail<suite_fail>(out.message());
        }
        catch (...)
        {
            handle_fail<suite_fail>("Необработанное нестандартное исключение.");
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
            out.print("Неожидаемое исключение " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                unexpected.label(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (std::exception& unexpected)
        {
            test::output out;
            out.print("Неожидаемое исключение: " DOT_TEST_OUTPUT_ANY, unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (...)
        {
            handle_fail<fail_type>("Неожидаемое исключение нестандартного класса.");
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
            out.print("Ожидается исключение " DOT_TEST_OUTPUT_ANY " но поймано исключение " DOT_TEST_OUTPUT_ANY ": " DOT_TEST_OUTPUT_ANY,
                exception_type::id().name(), unexpected.my_id().name(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (std::exception& unexpected)
        {
            test::output out;
            out.print("Ожидается исключение " DOT_TEST_OUTPUT_ANY " но поймано исключение: " DOT_TEST_OUTPUT_ANY,
                exception_type::id().name(), unexpected.what());
            handle_fail<fail_type>(out.message());
        }
        catch (...)
        {
            test::output out;
            out.print("Ожидается исключение " DOT_TEST_OUTPUT_ANY " но поймано исключение нестандартного класса.",
                exception_type::id().name());
            handle_fail<fail_type>(out.message());
        }
        test::output out;
        out.print("Ожидается исключение " DOT_TEST_OUTPUT_ANY " но никакое исключение не было брошено.",
            exception_type::id().name());
        handle_fail<fail_type>(out.message());
    }
}

// Здесь должен быть Unicode
