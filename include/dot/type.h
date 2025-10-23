// Основные типы использующиеся всей библиотекой
// идентификатор классов для динамической типизации
// классы приведения типов вверх по иерархии классов

#pragma once

#include <dot/public.h>
#include <dot/stdfwd.h>
#include <type_traits>
#include <cstdint>

namespace dot
{
    // знаковые целые
    typedef std::int64_t int64;
    typedef std::int32_t int32;
    typedef std::int16_t int16;
    typedef std::int8_t  int8;

    // беззнаковые целые
    typedef std::uint64_t uint64;
    typedef std::uint32_t uint32;
    typedef std::uint16_t uint16;
    typedef std::uint8_t  uint8;

    // беззнаковые целые без указания бит
    typedef unsigned int   uint;
    typedef unsigned short ushort;
    typedef unsigned long  ulong;

    // байт и знаковый байт
    typedef uint8 byte;
    typedef int8 sbyte;

    // класс для идентификации иерархического типа
    class DOT_PUBLIC class_id
    {
    public:
        explicit class_id(const char* const name) noexcept;

        // имя и уникальный индекс класса
        const char* const name() const noexcept;
        const uint64 index() const noexcept;

        // сравнение двух идентификаторов
        bool operator == (const class_id& another) const noexcept;
        bool operator != (const class_id& another) const noexcept;

    private:
        const char* const my_name;
        const uint64 my_index;
    };

    // запись идентификатора в поток вывода
    DOT_PUBLIC std::ostream& operator << (std::ostream& output, const class_id& identifier);

    // invalid_typecast() генерирует исключение приведения типов
    DOT_PUBLIC void invalid_typecast(const char* to_class, const char* from_class);

    // базовый класс для иерархии объектов и данных
    class DOT_PUBLIC hierarchic
    {
    public:
        virtual ~hierarchic() noexcept { }

        virtual const class_id& my_id() const noexcept = 0;

        // окончание рекурсивного поиска идентификатора в иерархии через шаблон
        template <typename instance_type>
        bool is() const noexcept
        {
            return is_base_id(instance_type::id());
        }

        // проверка что тип не является наследником указанного класса
        template <typename derived_type>
        bool is_not() const noexcept
        {
            return !is<derived_type>();
        }

        // окончание поиска идентификатора в иерархии через виртуальный метод
        virtual bool is_base_id(const class_id&) const noexcept
        {
            return false;
        }

        // приведение к типу с проверкой что данный экземпляр
        // является экземпляром либо потомком указанного класса
        // иначе генерируется исключение ошибки приведения типов
        template <typename derived_type>
        const derived_type& as() const
        {
            if (!is<derived_type>())
                invalid_typecast(derived_type::id().name(), my_id().name());
            return static_cast<const derived_type&>(*this);
        }

        template <typename derived_type>
        derived_type& as()
        {
            return const_cast<derived_type&>(
                static_cast<const hierarchic*>(this)->as<derived_type>());
        }
    };

    // -- проверка наследования не зависящая от RTTI --

    template <typename derived_type>
    struct is_class;

    // окончание рекурсии поиска предка шаблоном
    template<>
    struct is_class<hierarchic>
    {
        template <typename instance_type>
        static bool of()
        {
            return false;
        }
    };

    // проверка есть ли среди предков типа указаный тип
    template <typename derived_type>
    struct is_class
    {
        template <typename instance_type>
        static bool of() noexcept
        {
            return derived_type::id() == instance_type::id() ||
               is_class<typename derived_type::base>::template of<instance_type>();
        }
    };

// генерация объявления основных полей и методов класса в иерархии
#define DOT_HIERARCHIC(base_class) \
    typedef base_class base; \
    virtual bool is_base_id(const class_id& base_id) const noexcept override \
    { \
        return base_id == id() || \
               base::is_base_id(base_id); \
    } \
    virtual const class_id& my_id() const noexcept override \
    { \
        return id(); \
    } \
    static const class_id& id() noexcept

// генерация тела метода идентификатора класса в иерархии
#define DOT_CLASS_ID(class_name) \
    const class_id& class_name::id() noexcept \
    { \
        static const class_id identifier(#class_name); \
        return identifier; \
    }

    // шаблон для проверки можно ли тип записывать в поток вывода
    template <typename test_type, typename meta_type = void>
    struct writable_type : std::false_type { };

    template <typename test_type>
    struct writable_type<test_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<std::ostream&>() << std::declval<test_type>()),
            std::ostream&
        >>> : std::true_type { };

    template <typename test_type>
    inline constexpr bool is_writable = writable_type<test_type>::value;

    // шаблон для проверки можно ли тип считать из потока ввода
    template <typename test_type, typename meta_type = void>
    struct readable_type : std::false_type { };

    template <typename test_type>
    struct readable_type<test_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<std::istream&>() >> std::declval<test_type&>()),
            std::istream&
        >>> : std::true_type { };

    template <typename test_type>
    inline constexpr bool is_readable = readable_type<test_type>::value;

    // шаблон для проверки поддерживают ли типы операцию сравнения
    template <typename left_type, typename right_type, typename meta_type = void>
    struct comparable_types : std::false_type { };

    template <typename left_type, typename right_type>
    struct comparable_types<typename left_type, typename right_type,
        std::enable_if_t<std::is_convertible_v<
            decltype(std::declval<left_type>() == std::declval<right_type>()),
            bool
        >>> : std::true_type { };

    template <typename left_type, typename right_type>
    inline constexpr bool are_comparable = comparable_types<left_type, right_type>::value;

    template <typename test_type>
    inline constexpr bool is_comparable = comparable_types<test_type, test_type>::value;

    // шаблон для проверки поддерживают ли типы операцию упорядочивания
    template <typename left_type, typename right_type, typename meta_type = void>
    struct orderable_types : std::false_type { };

    template <typename left_type, typename right_type>
    struct orderable_types<left_type, right_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<left_type>() < std::declval<right_type>()),
            bool
        >>> : std::true_type { };

    template <typename left_type, typename right_type>
    inline constexpr bool are_orderable = orderable_types<left_type, right_type>::value;

    template <typename test_type>
    inline constexpr bool is_orderable = orderable_types<test_type, test_type>::value;
}

// Здесь должен быть Unicode
