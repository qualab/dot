// DOT scalar data object

#include <dot/scalar.h>

namespace dot
{
    scalar::scalar()
    {
    }

    const class_id& scalar::id() noexcept
    {
        static const class_id scalar_id("scalar");
        return scalar_id;
    }

    const class_id& scalar::data::id() noexcept
    {
        static const class_id scalar_data_id("scalar::data");
        return scalar_data_id;
    }

    template<> const class_id& scalar_of<long long>::id() noexcept
    {
        static const class_id scalar_long_long_id("scalar_of<long long>");
        return scalar_long_long_id;
    }

    template<> const class_id& scalar_of<long>::id() noexcept
    {
        static const class_id scalar_long_id("scalar_of<long>");
        return scalar_long_id;
    }

    template<> const class_id& scalar_of<int>::id() noexcept
    {
        static const class_id scalar_int_id("scalar_of<int>");
        return scalar_int_id;
    }

    template<> const class_id& scalar_of<short>::id() noexcept
    {
        static const class_id scalar_short_id("scalar_of<short>");
        return scalar_short_id;
    }

    template<> const class_id& scalar_of<char>::id() noexcept
    {
        static const class_id scalar_char_id("scalar_of<char>");
        return scalar_char_id;
    }

    template<> const class_id& scalar_of<unsigned long long>::id() noexcept
    {
        static const class_id scalar_unsigned_long_long_id("scalar_of<unsigned long long>");
        return scalar_unsigned_long_long_id;
    }

    template<> const class_id& scalar_of<unsigned long>::id() noexcept
    {
        static const class_id scalar_unsigned_long_id("scalar_of<unsigned long>");
        return scalar_unsigned_long_id;
    }

    template<> const class_id& scalar_of<unsigned int>::id() noexcept
    {
        static const class_id scalar_unsigned_int_id("scalar_of<unsigned int>");
        return scalar_unsigned_int_id;
    }

    template<> const class_id& scalar_of<unsigned short>::id() noexcept
    {
        static const class_id scalar_unsigned_short_id("scalar_of<unsigned short>");
        return scalar_unsigned_short_id;
    }

    template<> const class_id& scalar_of<unsigned char>::id() noexcept
    {
        static const class_id scalar_unsigned_char_id("scalar_of<unsigned char>");
        return scalar_unsigned_char_id;
    }

    template<> const class_id& scalar_of<double>::id() noexcept
    {
        static const class_id scalar_double_id("scalar_of<double>");
        return scalar_double_id;
    }

    template<> const class_id& scalar_of<float >::id() noexcept
    {
        static const class_id scalar_float_id("scalar_of<float>");
        return scalar_float_id;
    }

    template<> const class_id& scalar_of<bool>::id() noexcept
    {
        static const class_id scalar_bool_id("scalar_of<bool>");
        return scalar_bool_id;
    }

    template<> const class_id& scalar_of<long long>::data::id() noexcept
    {
        static const class_id scalar_long_long_data_id("scalar_of<long long>::data");
        return scalar_long_long_data_id;
    }

    template<> const class_id& scalar_of<long>::data::id() noexcept
    {
        static const class_id scalar_long_data_id("scalar_of<long>::data");
        return scalar_long_data_id;
    }

    template<> const class_id& scalar_of<int>::data::id() noexcept
    {
        static const class_id scalar_int_data_id("scalar_of<int>::data");
        return scalar_int_data_id;
    }

    template<> const class_id& scalar_of<short>::data::id() noexcept
    {
        static const class_id scalar_short_data_id("scalar_of<short>::data");
        return scalar_short_data_id;
    }

    template<> const class_id& scalar_of<char>::data::id() noexcept
    {
        static const class_id scalar_char_data_id("scalar_of<char>::data");
        return scalar_char_data_id;
    }

    template<> const class_id& scalar_of<unsigned long long>::data::id() noexcept
    {
        static const class_id scalar_unsigned_long_long_data_id("scalar_of<unsigned long long>::data");
        return scalar_unsigned_long_long_data_id;
    }

    template<> const class_id& scalar_of<unsigned long>::data::id() noexcept
    {
        static const class_id scalar_unsigned_long_data_id("scalar_of<unsigned long>::data");
        return scalar_unsigned_long_data_id;
    }

    template<> const class_id& scalar_of<unsigned int>::data::id() noexcept
    {
        static const class_id scalar_unsigned_int_data_id("scalar_of<unsigned int>::data");
        return scalar_unsigned_int_data_id;
    }

    template<> const class_id& scalar_of<unsigned short>::data::id() noexcept
    {
        static const class_id scalar_unsigned_short_data_id("scalar_of<unsigned short>::data");
        return scalar_unsigned_short_data_id;
    }

    template<> const class_id& scalar_of<unsigned char>::data::id() noexcept
    {
        static const class_id scalar_unsigned_char_data_id("scalar_of<unsigned char>::data");
        return scalar_unsigned_char_data_id;
    }

    template<> const class_id& scalar_of<double>::data::id() noexcept
    {
        static const class_id scalar_double_data_id("scalar_of<double>::data");
        return scalar_double_data_id;
    }

    template<> const class_id& scalar_of<float>::data::id() noexcept
    {
        static const class_id scalar_float_data_id("scalar_of<float>::data");
        return scalar_float_data_id;
    }

    template<> const class_id& scalar_of<bool>::data::id() noexcept
    {
        static const class_id scalar_bool_data_id("scalar_of<bool>::data");
        return scalar_bool_data_id;
    }
}

// Unicode signature: Владимир Керимов
