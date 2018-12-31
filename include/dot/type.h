// DOT public declaration

#pragma once

#include <dot/public.h>
#include <type_traits>
#include <cstdint>
#include <iosfwd>

namespace dot
{
    // signed integers
    typedef std::int64_t int64;
    typedef std::int32_t int32;
    typedef std::int16_t int16;
    typedef std::int8_t  int8;

    // unsigned integers
    typedef std::uint64_t uint64;
    typedef std::uint32_t uint32;
    typedef std::uint16_t uint16;
    typedef std::uint8_t  uint8;

    // unsigned integers
    typedef unsigned int   uint;
    typedef unsigned short ushort;
    typedef unsigned long  ulong;

    // byte types
    typedef uint8 byte;
    typedef int8 sbyte;

    // class identificator type
    class DOT_PUBLIC class_id
    {
    public:
        class_id(const char* const name) noexcept;

        const char* const name() const noexcept;
        const uint64 index() const noexcept;

        bool operator == (const class_id& another) const noexcept;
        bool operator != (const class_id& another) const noexcept;

    private:
        const char* const my_name;
        const uint64 my_index;
    };

    // -- to avoid case when no RTTI allowed --

    // is class derived from another check
    template <typename derived_type>
    struct is_class
    {
        template <typename instance_type>
        static bool of() noexcept
        {
            return derived_type::id() == instance_type::id() ||
               is_class<derived_type::base>::of<instance_type>();
        }
    };

    // invalid_typecast() throws invalid type cast exception by implementation
    DOT_PUBLIC void invalid_typecast(const char* to_class, const char* from_class);

    // base class for any hierarchy with runtime up-casts
    class DOT_PUBLIC hierarchic
    {
    public:
        virtual ~hierarchic() noexcept { }

        virtual const class_id& who() const noexcept = 0;

        // hierarchy base class ends instance hierarchy search
        template <typename instance_type>
        bool is() const noexcept
        {
            return is_base_id(instance_type::id());
        }

        template <typename derived_type>
        bool is_not() const noexcept
        {
            return !is<derived_type>();
        }

        // hierarchicy base class ends instance hierarchy search
        virtual bool is_base_id(const class_id&) const noexcept
        {
            return false;
        }

        template <typename derived_type>
        const derived_type& as() const
        {
            if (!is<derived_type>())
                invalid_typecast(derived_type::id().name(), who().name());
            return static_cast<const derived_type&>(*this);
        }

        template <typename derived_type>
        derived_type& as()
        {
            return const_cast<derived_type&>(
                static_cast<const hierarchic*>(this)->as<derived_type>());
        }
    };

    // hierarchicy base class ends class hierarchy search
    template<>
    struct is_class<hierarchic>
    {
        template <typename instance_type>
        static bool of()
        {
            return false;
        }
    };

#define DOT_HIERARCHIC(base_class) \
    typedef base_class base; \
    virtual bool is_base_id(const class_id& base_id) const noexcept override \
    { \
        return base_id == id() || \
               base::is_base_id(base_id); \
    } \
    static const class_id& id() noexcept; \
    virtual const class_id& who() const noexcept override

    // check is the type able to be written into output byte stream
    template <typename test_type, typename meta_type = void>
    struct writable_type : std::false_type { };

    template <typename test_type>
    struct writable_type<test_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<std::ostream&>() << std::declval<test_type>()),
            std::ostream&
        >>> : std::true_type { };

    template <typename test_type>
    constexpr bool is_writable = writable_type<test_type>::value;

    // check is the type able to be read from input byte stream
    template <typename test_type, typename meta_type = void>
    struct readable_type : std::false_type { };

    template <typename test_type>
    struct readable_type<test_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<std::istream&>() >> std::declval<test_type&>()),
            std::istream&
        >>> : std::true_type { };

    template <typename test_type>
    constexpr bool is_readable = readable_type<test_type>::value;

    // check is the type support the operation of comparison
    template <typename left_type, typename right_type, typename meta_type = void>
    struct comparable_types : std::false_type { };

    template <typename left_type, typename right_type>
    struct comparable_types<typename left_type, typename right_type,
        std::enable_if_t<std::is_convertible_v<
            decltype(std::declval<left_type>() == std::declval<right_type>()),
            bool
        >>> : std::true_type { };

    template <typename left_type, typename right_type>
    constexpr bool are_comparable = comparable_types<left_type, right_type>::value;

    template <typename test_type>
    constexpr bool is_comparable = comparable_types<test_type, test_type>::value;

    // check is the type supports the operation of ordering
    template <typename left_type, typename right_type, typename meta_type = void>
    struct orderable_types : std::false_type { };

    template <typename left_type, typename right_type>
    struct orderable_types<left_type, right_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<left_type>() < std::declval<right_type>()),
            bool
        >>> : std::true_type { };

    template <typename left_type, typename right_type>
    static constexpr bool are_orderable = orderable_types<left_type, right_type>::value;

    template <typename test_type>
    static constexpr bool is_orderable = orderable_types<test_type, test_type>::value;
}

// Unicode signature: Владимир Керимов
