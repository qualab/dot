// dot library macro for public declarations
// for windows only must be defined DOT_EXPORTS
// in case of build dynamic link library
// define DOT_STATIC to build static link library

#pragma once

// fixes CMake bug
#ifdef dot_STATIC
#define DOT_STATIC
#endif

// fixes CMake bug
#ifdef dot_EXPORTS
#define DOT_EXPORTS
#endif

// public declarations marked as DOT_PUBLIC
#if !defined(_WIN32) || defined(DOT_STATIC)
#   define DOT_PUBLIC
#elif defined(DOT_EXPORTS)
#   define DOT_PUBLIC __declspec(dllexport)
#else
#   define DOT_PUBLIC __declspec(dllimport)
#endif

namespace dot
{
    // base type for any high-level class
    class object;

    // base class for any box for cat of any shape
    class box_based;

    // each box contains liquid cat of proper shape
    template <class shape>
    class box;

    // base class for any rope bound to cow
    class rope_based;

    // each rope is bound to some cow to copy-on-write
    template <class instance>
    class rope;
}

// Здесь должен быть Unicode
