// DOT public declaration

#pragma once

#include <dot/public.h>
#include <cstdint>

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

    // class identificator type
    typedef const char* const class_name_type;

    // -- to avoid case when no RTTI allowed --

    // base structure for runtime class identification
    struct class_id { };

    // is class derived from another check
    template <typename derived_type>
    struct is_class : class_id
    {
        template <typename base_type>
        static bool of()
        {
            return derived_type::class_name == base_type::class_name ||
                is_class<derived_type::base>::of<base_type>();
        }
    };

    // invalid_typecast() throws invalid type cast exception by implementation
    DOT_PUBLIC void invalid_typecast(class_name_type to_type_name, class_name_type from_type_name);

    // base class for any hierarchy with runtime up-casts
    class DOT_PUBLIC basement
    {
    public:
        virtual ~basement() { }

        virtual class_name_type get_class_name() const = 0;
        virtual const class_id& get_class_id() const = 0;

        template <typename derived_type>
        bool is() const
        {
            return get_class_id().of<derived_type>();
        }

        template <typename derived_type>
        const derived_type& as() const
        {
            if (!is<derived_type>())
                invalid_typecast(derived_type::class_name, get_class_name());
            return static_cast<const derived_type&>(*this);
        }

        template <typename derived_type>
        derived_type& as()
        {
            return const_cast<derived_type&>(
                static_cast<const object*>(this)->as<derived_type>());
        }
    };

    // basement ends hierarchy search
    template<>
    struct is_class<basement>
    {
        template <typename base_type>
        static bool of()
        {
            return false;
        }
    };
}

// forward declarations of std::string
// without include of <string>
// to improve compilation speed
namespace std
{
    template <typename char_type>
    struct char_traits;

    template <typename char_type>
    class allocator;

    template <typename char_type,
              typename traits_type=char_traits<char_type>,
              typename allocator_type=allocator<char_type>>
    class basic_string;

    typedef basic_string<char>     string;
    typedef basic_string<wchar_t>  wstring;
    typedef basic_string<char16_t> u16string;
    typedef basic_string<char32_t> u32string;
}

// Unicode signature: Владимир Керимов
