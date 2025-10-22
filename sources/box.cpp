// DOT box data object

#include <dot/box.h>

namespace dot
{
    DOT_CLASS_ID(box_based)
    DOT_CLASS_ID(box_based::cat_based)

    template<> DOT_CLASS_ID(box<long long>)
    template<> DOT_CLASS_ID(box<long>)
    template<> DOT_CLASS_ID(box<int>)
    template<> DOT_CLASS_ID(box<short>)
    template<> DOT_CLASS_ID(box<char>)

    template<> DOT_CLASS_ID(box<unsigned long long>)
    template<> DOT_CLASS_ID(box<unsigned long>)
    template<> DOT_CLASS_ID(box<unsigned int>)
    template<> DOT_CLASS_ID(box<unsigned short>)
    template<> DOT_CLASS_ID(box<unsigned char>)

    template<> DOT_CLASS_ID(box<double>)
    template<> DOT_CLASS_ID(box<float>)
    template<> DOT_CLASS_ID(box<bool>)

    template<> DOT_CLASS_ID(box<long long>::cat)
    template<> DOT_CLASS_ID(box<long>::cat)
    template<> DOT_CLASS_ID(box<int>::cat)
    template<> DOT_CLASS_ID(box<short>::cat)
    template<> DOT_CLASS_ID(box<char>::cat)

    template<> DOT_CLASS_ID(box<unsigned long long>::cat)
    template<> DOT_CLASS_ID(box<unsigned long>::cat)
    template<> DOT_CLASS_ID(box<unsigned int>::cat)
    template<> DOT_CLASS_ID(box<unsigned short>::cat)
    template<> DOT_CLASS_ID(box<unsigned char>::cat)

    template<> DOT_CLASS_ID(box<double>::cat)
    template<> DOT_CLASS_ID(box<float>::cat)
    template<> DOT_CLASS_ID(box<bool>::cat)
}

// Здесь должен быть Unicode
