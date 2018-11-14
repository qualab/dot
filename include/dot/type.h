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
        template <typename base_type>
        static bool of() noexcept
        {
            return derived_type::id() == base_type::id() ||
               is_class<derived_type::base>::of<base_type>();
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

        template <typename derived_type>
        bool is() const noexcept
        {
            return derived_type::id() == who() || is<derived_type::base>();
        }

        template <typename derived_type>
        bool is_not() const noexcept
        {
            return !is<derived_type>();
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
        template <typename base_type>
        static bool of()
        {
            return false;
        }
    };

    // hierarchy base class ends instance hierarchy search
    template<>
    inline bool hierarchic::is<hierarchic>() const noexcept
    {
        return false;
    }

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
    template <typename test_type, typename meta_type = void>
    struct comparable_type : std::false_type { };

    template <typename test_type>
    struct comparable_type<test_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<test_type>() == std::declval<test_type>()),
            bool
        >>> : std::true_type { };

    template <typename test_type>
    constexpr bool is_comparable = comparable_type<test_type>::value;

    // check is the type supports the operation of ordering
    template <typename test_type, typename meta_type = void>
    struct orderable_type : std::false_type { };

    template <typename test_type>
    struct orderable_type<test_type, std::enable_if_t<
        std::is_convertible_v<
            decltype(std::declval<test_type>() < std::declval<test_type>()),
            bool
        >>> : std::true_type { };

    template <typename test_type>
    static constexpr bool is_orderable = orderable_type<test_type>::value;
}

// Unicode signature: Владимир Керимов
