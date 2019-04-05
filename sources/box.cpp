// DOT box data object

#include <dot/box.h>

namespace dot
{
    box_based::box_based()
    {
    }

    const class_id& box_based::id() noexcept
    {
        static const class_id boxes_id("box_based");
        return boxes_id;
    }

    const class_id& box_based::cat_based::id() noexcept
    {
        static const class_id boxes_cats_id("box_based::cat_based");
        return boxes_cats_id;
    }

    template<> const class_id& box<long long>::id() noexcept
    {
        static const class_id box_long_long_id("box<long long>");
        return box_long_long_id;
    }

    template<> const class_id& box<long>::id() noexcept
    {
        static const class_id box_long_id("box<long>");
        return box_long_id;
    }

    template<> const class_id& box<int>::id() noexcept
    {
        static const class_id box_int_id("box<int>");
        return box_int_id;
    }

    template<> const class_id& box<short>::id() noexcept
    {
        static const class_id box_short_id("box<short>");
        return box_short_id;
    }

    template<> const class_id& box<char>::id() noexcept
    {
        static const class_id box_char_id("box<char>");
        return box_char_id;
    }

    template<> const class_id& box<unsigned long long>::id() noexcept
    {
        static const class_id box_unsigned_long_long_id("box<unsigned long long>");
        return box_unsigned_long_long_id;
    }

    template<> const class_id& box<unsigned long>::id() noexcept
    {
        static const class_id box_unsigned_long_id("box<unsigned long>");
        return box_unsigned_long_id;
    }

    template<> const class_id& box<unsigned int>::id() noexcept
    {
        static const class_id box_unsigned_int_id("box<unsigned int>");
        return box_unsigned_int_id;
    }

    template<> const class_id& box<unsigned short>::id() noexcept
    {
        static const class_id box_unsigned_short_id("box<unsigned short>");
        return box_unsigned_short_id;
    }

    template<> const class_id& box<unsigned char>::id() noexcept
    {
        static const class_id box_unsigned_char_id("box<unsigned char>");
        return box_unsigned_char_id;
    }

    template<> const class_id& box<double>::id() noexcept
    {
        static const class_id box_double_id("box<double>");
        return box_double_id;
    }

    template<> const class_id& box<float >::id() noexcept
    {
        static const class_id box_float_id("box<float>");
        return box_float_id;
    }

    template<> const class_id& box<bool>::id() noexcept
    {
        static const class_id box_bool_id("box<bool>");
        return box_bool_id;
    }

    template<> const class_id& box<long long>::cat::id() noexcept
    {
        static const class_id box_long_long_cat_id("box<long long>::cat");
        return box_long_long_cat_id;
    }

    template<> const class_id& box<long>::cat::id() noexcept
    {
        static const class_id box_long_cat_id("box<long>::cat");
        return box_long_cat_id;
    }

    template<> const class_id& box<int>::cat::id() noexcept
    {
        static const class_id box_int_cat_id("box<int>::cat");
        return box_int_cat_id;
    }

    template<> const class_id& box<short>::cat::id() noexcept
    {
        static const class_id box_short_cat_id("box<short>::cat");
        return box_short_cat_id;
    }

    template<> const class_id& box<char>::cat::id() noexcept
    {
        static const class_id box_char_cat_id("box<char>::cat");
        return box_char_cat_id;
    }

    template<> const class_id& box<unsigned long long>::cat::id() noexcept
    {
        static const class_id box_unsigned_long_long_cat_id("box<unsigned long long>::cat");
        return box_unsigned_long_long_cat_id;
    }

    template<> const class_id& box<unsigned long>::cat::id() noexcept
    {
        static const class_id box_unsigned_long_cat_id("box<unsigned long>::cat");
        return box_unsigned_long_cat_id;
    }

    template<> const class_id& box<unsigned int>::cat::id() noexcept
    {
        static const class_id box_unsigned_int_cat_id("box<unsigned int>::cat");
        return box_unsigned_int_cat_id;
    }

    template<> const class_id& box<unsigned short>::cat::id() noexcept
    {
        static const class_id box_unsigned_short_cat_id("box<unsigned short>::cat");
        return box_unsigned_short_cat_id;
    }

    template<> const class_id& box<unsigned char>::cat::id() noexcept
    {
        static const class_id box_unsigned_char_cat_id("box<unsigned char>::cat");
        return box_unsigned_char_cat_id;
    }

    template<> const class_id& box<double>::cat::id() noexcept
    {
        static const class_id box_double_cat_id("box<double>::cat");
        return box_double_cat_id;
    }

    template<> const class_id& box<float>::cat::id() noexcept
    {
        static const class_id box_float_cat_id("box<float>::cat");
        return box_float_cat_id;
    }

    template<> const class_id& box<bool>::cat::id() noexcept
    {
        static const class_id box_bool_cat_id("box<bool>::cat");
        return box_bool_cat_id;
    }
}

// Здесь должен быть Unicode
