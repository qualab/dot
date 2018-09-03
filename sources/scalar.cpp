// DOT scalar data object

#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    template<> const class_id& scalar<int64>::id() noexcept
    {
        static const class_id scalar_int64_id("scalar<int64>");
        return scalar_int64_id;
    }

    template<> const class_id& scalar<int32>::id() noexcept
    {
        static const class_id scalar_int32_id("scalar<int32>");
        return scalar_int32_id;
    }

    template<> const class_id& scalar<int16>::id() noexcept
    {
        static const class_id scalar_int16_id("scalar<int16>");
        return scalar_int16_id;
    }

    template<> const class_id& scalar<int8>::id() noexcept
    {
        static const class_id scalar_int8_id("scalar<int8>");
        return scalar_int8_id;
    }

    template<> const class_id& scalar<uint64>::id() noexcept
    {
        static const class_id scalar_uint64_id("scalar<uint64>");
        return scalar_uint64_id;
    }

    template<> const class_id& scalar<uint32>::id() noexcept
    {
        static const class_id scalar_uint32_id("scalar<uint32>");
        return scalar_uint32_id;
    }

    template<> const class_id& scalar<uint16>::id() noexcept
    {
        static const class_id scalar_uint16_id("scalar<uint16>");
        return scalar_uint16_id;
    }

    template<> const class_id& scalar<uint8 >::id() noexcept
    {
        static const class_id scalar_uint8_id("scalar<uint8>");
        return scalar_uint8_id;
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

    template<> const class_id& scalar<char>::id() noexcept
    {
        static const class_id scalar_char_id("scalar<char>");
        return scalar_char_id;
    }

    template<> const class_id& scalar<int64>::data::id() noexcept
    {
        static const class_id scalar_int64_data_id("scalar<int64>::data");
        return scalar_int64_data_id;
    }

    template<> const class_id& scalar<int32>::data::id() noexcept
    {
        static const class_id scalar_int32_data_id("scalar<int32>::data");
        return scalar_int32_data_id;
    }

    template<> const class_id& scalar<int16>::data::id() noexcept
    {
        static const class_id scalar_int16_data_id("scalar<int16>::data");
        return scalar_int16_data_id;
    }

    template<> const class_id& scalar<int8>::data::id() noexcept
    {
        static const class_id scalar_int8_data_id("scalar<int8>::data");
        return scalar_int8_data_id;
    }

    template<> const class_id& scalar<uint64>::data::id() noexcept
    {
        static const class_id scalar_uint64_data_id("scalar<uint64>::data");
        return scalar_uint64_data_id;
    }

    template<> const class_id& scalar<uint32>::data::id() noexcept
    {
        static const class_id scalar_uint32_data_id("scalar<uint32>::data");
        return scalar_uint32_data_id;
    }

    template<> const class_id& scalar<uint16>::data::id() noexcept
    {
        static const class_id scalar_uint16_data_id("scalar<uint16>::data");
        return scalar_uint16_data_id;
    }

    template<> const class_id& scalar<uint8>::data::id() noexcept
    {
        static const class_id scalar_uint8_data_id("scalar<uint8>::data");
        return scalar_uint8_data_id;
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

    template<> const class_id& scalar<char>::data::id() noexcept
    {
        static const class_id scalar_char_data_id("scalar<char>::data");
        return scalar_char_data_id;
    }
}

// Unicode signature: Владимир Керимов
