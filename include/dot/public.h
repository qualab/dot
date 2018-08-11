// DOT public declaration

#pragma once

#if !defined(_WIN32) || defined(DOT_STATIC)
#   define DYN_PUBLIC
#elif defined(DOT_EXPORTS)
#   define DOT_PUBLIC __declspec(dllexport)
#else
#   define DOT_PUBLIC __declspec(dllimport)
#endif

// Unicode signature: Владимир Керимов
