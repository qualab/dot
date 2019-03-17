// DOT scalar data object

#include <dot/scalar.h>

namespace dot
{
    boxes::boxes()
    {
    }

    const class_id& boxes::id() noexcept
    {
        static const class_id scalar_id("boxes");
        return scalar_id;
    }

    const class_id& boxes::data::id() noexcept
    {
        static const class_id scalar_data_id("boxes::data");
        return scalar_data_id;
    }

    template<> const class_id& box<long long>::id() noexcept
    {
        static const class_id scalar_long_long_id("box<long long>");
        return scalar_long_long_id;
    }

    template<> const class_id& box<long>::id() noexcept
    {
        static const class_id scalar_long_id("box<long>");
        return scalar_long_id;
    }

    template<> const class_id& box<int>::id() noexcept
    {
        static const class_id scalar_int_id("box<int>");
        return scalar_int_id;
    }

    template<> const class_id& box<short>::id() noexcept
    {
        static const class_id scalar_short_id("box<short>");
        return scalar_short_id;
    }

    template<> const class_id& box<char>::id() noexcept
    {
        static const class_id scalar_char_id("box<char>");
        return scalar_char_id;
    }

    template<> const class_id& box<unsigned long long>::id() noexcept
    {
        static const class_id scalar_unsigned_long_long_id("box<unsigned long long>");
        return scalar_unsigned_long_long_id;
    }

    template<> const class_id& box<unsigned long>::id() noexcept
    {
        static const class_id scalar_unsigned_long_id("box<unsigned long>");
        return scalar_unsigned_long_id;
    }

    template<> const class_id& box<unsigned int>::id() noexcept
    {
        static const class_id scalar_unsigned_int_id("box<unsigned int>");
        return scalar_unsigned_int_id;
    }

    template<> const class_id& box<unsigned short>::id() noexcept
    {
        static const class_id scalar_unsigned_short_id("box<unsigned short>");
        return scalar_unsigned_short_id;
    }

    template<> const class_id& box<unsigned char>::id() noexcept
    {
        static const class_id scalar_unsigned_char_id("box<unsigned char>");
        return scalar_unsigned_char_id;
    }

    template<> const class_id& box<double>::id() noexcept
    {
        static const class_id scalar_double_id("box<double>");
        return scalar_double_id;
    }

    template<> const class_id& box<float >::id() noexcept
    {
        static const class_id scalar_float_id("box<float>");
        return scalar_float_id;
    }

    template<> const class_id& box<bool>::id() noexcept
    {
        static const class_id scalar_bool_id("box<bool>");
        return scalar_bool_id;
    }

    template<> const class_id& box<long long>::data::id() noexcept
    {
        static const class_id scalar_long_long_data_id("box<long long>::data");
        return scalar_long_long_data_id;
    }

    template<> const class_id& box<long>::data::id() noexcept
    {
        static const class_id scalar_long_data_id("box<long>::data");
        return scalar_long_data_id;
    }

    template<> const class_id& box<int>::data::id() noexcept
    {
        static const class_id scalar_int_data_id("box<int>::data");
        return scalar_int_data_id;
    }

    template<> const class_id& box<short>::data::id() noexcept
    {
        static const class_id scalar_short_data_id("box<short>::data");
        return scalar_short_data_id;
    }

    template<> const class_id& box<char>::data::id() noexcept
    {
        static const class_id scalar_char_data_id("box<char>::data");
        return scalar_char_data_id;
    }

    template<> const class_id& box<unsigned long long>::data::id() noexcept
    {
        static const class_id scalar_unsigned_long_long_data_id("box<unsigned long long>::data");
        return scalar_unsigned_long_long_data_id;
    }

    template<> const class_id& box<unsigned long>::data::id() noexcept
    {
        static const class_id scalar_unsigned_long_data_id("box<unsigned long>::data");
        return scalar_unsigned_long_data_id;
    }

    template<> const class_id& box<unsigned int>::data::id() noexcept
    {
        static const class_id scalar_unsigned_int_data_id("box<unsigned int>::data");
        return scalar_unsigned_int_data_id;
    }

    template<> const class_id& box<unsigned short>::data::id() noexcept
    {
        static const class_id scalar_unsigned_short_data_id("box<unsigned short>::data");
        return scalar_unsigned_short_data_id;
    }

    template<> const class_id& box<unsigned char>::data::id() noexcept
    {
        static const class_id scalar_unsigned_char_data_id("box<unsigned char>::data");
        return scalar_unsigned_char_data_id;
    }

    template<> const class_id& box<double>::data::id() noexcept
    {
        static const class_id scalar_double_data_id("box<double>::data");
        return scalar_double_data_id;
    }

    template<> const class_id& box<float>::data::id() noexcept
    {
        static const class_id scalar_float_data_id("box<float>::data");
        return scalar_float_data_id;
    }

    template<> const class_id& box<bool>::data::id() noexcept
    {
        static const class_id scalar_bool_data_id("box<bool>::data");
        return scalar_bool_data_id;
    }
}

// Здесь должен быть Unicode
