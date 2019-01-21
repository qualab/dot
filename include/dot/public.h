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

// Здесь должен быть Unicode
