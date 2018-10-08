// DOT scalar data object

#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    template<> const class_id& scalar<long long>::id() noexcept
    {
        static const class_id scalar_long_long_id("scalar<long long>");
        return scalar_long_long_id;
    }

    template<> const class_id& scalar<long>::id() noexcept
    {
        static const class_id scalar_long_id("scalar<long>");
        return scalar_long_id;
    }

    template<> const class_id& scalar<int>::id() noexcept
    {
        static const class_id scalar_int_id("scalar<int>");
        return scalar_int_id;
    }

    template<> const class_id& scalar<short>::id() noexcept
    {
        static const class_id scalar_short_id("scalar<short>");
        return scalar_short_id;
    }

    template<> const class_id& scalar<char>::id() noexcept
    {
        static const class_id scalar_char_id("scalar<char>");
        return scalar_char_id;
    }

    template<> const class_id& scalar<unsigned long long>::id() noexcept
    {
        static const class_id scalar_unsigned_long_long_id("scalar<unsigned long long>");
        return scalar_unsigned_long_long_id;
    }

    template<> const class_id& scalar<unsigned long>::id() noexcept
    {
        static const class_id scalar_unsigned_long_id("scalar<unsigned long>");
        return scalar_unsigned_long_id;
    }

    template<> const class_id& scalar<unsigned int>::id() noexcept
    {
        static const class_id scalar_unsigned_int_id("scalar<unsigned int>");
        return scalar_unsigned_int_id;
    }

    template<> const class_id& scalar<unsigned short>::id() noexcept
    {
        static const class_id scalar_unsigned_short_id("scalar<unsigned short>");
        return scalar_unsigned_short_id;
    }

    template<> const class_id& scalar<unsigned char>::id() noexcept
    {
        static const class_id scalar_unsigned_char_id("scalar<unsigned char>");
        return scalar_unsigned_char_id;
    }

    template<> const class_id& scalar<double>::id() noexcept
    {
        static const class_id scalar_double_id("scalar<double>");
        return scalar_double_id;
    }

    template<> const class_id& scalar<float >::id() noexcept
    {
        static const class_id scalar_float_id("scalar<float>");
        return scalar_float_id;
    }

    template<> const class_id& scalar<bool>::id() noexcept
    {
        static const class_id scalar_bool_id("scalar<bool>");
        return scalar_bool_id;
    }

    template<> const class_id& scalar<long long>::data::id() noexcept
    {
        static const class_id scalar_long_long_data_id("scalar<long long>::data");
        return scalar_long_long_data_id;
    }

    template<> const class_id& scalar<long>::data::id() noexcept
    {
        static const class_id scalar_long_data_id("scalar<long>::data");
        return scalar_long_data_id;
    }

    template<> const class_id& scalar<int>::data::id() noexcept
    {
        static const class_id scalar_int_data_id("scalar<int>::data");
        return scalar_int_data_id;
    }

    template<> const class_id& scalar<short>::data::id() noexcept
    {
        static const class_id scalar_short_data_id("scalar<short>::data");
        return scalar_short_data_id;
    }

    template<> const class_id& scalar<char>::data::id() noexcept
    {
        static const class_id scalar_char_data_id("scalar<char>::data");
        return scalar_char_data_id;
    }

    template<> const class_id& scalar<unsigned long long>::data::id() noexcept
    {
        static const class_id scalar_unsigned_long_long_data_id("scalar<unsigned long long>::data");
        return scalar_unsigned_long_long_data_id;
    }

    template<> const class_id& scalar<unsigned long>::data::id() noexcept
    {
        static const class_id scalar_unsigned_long_data_id("scalar<unsigned long>::data");
        return scalar_unsigned_long_data_id;
    }

    template<> const class_id& scalar<unsigned int>::data::id() noexcept
    {
        static const class_id scalar_unsigned_int_data_id("scalar<unsigned int>::data");
        return scalar_unsigned_int_data_id;
    }

    template<> const class_id& scalar<unsigned short>::data::id() noexcept
    {
        static const class_id scalar_unsigned_short_data_id("scalar<unsigned short>::data");
        return scalar_unsigned_short_data_id;
    }

    template<> const class_id& scalar<unsigned char>::data::id() noexcept
    {
        static const class_id scalar_unsigned_char_data_id("scalar<unsigned char>::data");
        return scalar_unsigned_char_data_id;
    }

    template<> const class_id& scalar<double>::data::id() noexcept
    {
        static const class_id scalar_double_data_id("scalar<double>::data");
        return scalar_double_data_id;
    }

    template<> const class_id& scalar<float>::data::id() noexcept
    {
        static const class_id scalar_float_data_id("scalar<float>::data");
        return scalar_float_data_id;
    }

    template<> const class_id& scalar<bool>::data::id() noexcept
    {
        static const class_id scalar_bool_data_id("scalar<bool>::data");
        return scalar_bool_data_id;
    }
}

// Unicode signature: Владимир Керимов
