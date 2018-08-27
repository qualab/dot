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

    // is class derived from another check
    template <typename derived_type>
    struct is_class
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
    class DOT_PUBLIC hierarchic
    {
    public:
        virtual ~hierarchic() { }

        virtual class_name_type who() const = 0;

        template <typename derived_type>
        bool is() const
        {
            return derived_type::class_name == who() || is<derived_type::base>();
        }

        template <typename derived_type>
        bool is_not() const
        {
            return !is<derived_type>();
        }

        template <typename derived_type>
        const derived_type& as() const
        {
            if (!is<derived_type>())
                invalid_typecast(derived_type::class_name, who());
            return static_cast<const derived_type&>(*this);
        }

        template <typename derived_type>
        derived_type& as()
        {
            return const_cast<derived_type&>(
                static_cast<const object*>(this)->as<derived_type>());
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
    inline bool hierarchic::is<hierarchic>() const
    {
        return false;
    }
}

// Unicode signature: Владимир Керимов
