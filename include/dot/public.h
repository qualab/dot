// dot library macro for public declarations
// for windows only must be defined DOT_EXPORTS
// in case of build dynamic link library
// define DOT_STATIC to build static link library

#pragma once

#if !defined(_WIN32) || defined(DOT_STATIC)
#   define DYN_PUBLIC
#elif defined(DOT_EXPORTS)
#   define DOT_PUBLIC __declspec(dllexport)
#else
#   define DOT_PUBLIC __declspec(dllimport)
#endif

namespace dot
{
    class object;

    class scalar;
    template <typename value_type> class scalar_of;

    class copyable;
    template <typename instance_type> class copyable_of;
}

// Здесь должен быть Unicode
