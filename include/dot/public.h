// Необходимые макросы для доступа к типам
// и объявления базовых типов объектов

#pragma once

// для CMake генерации
#ifdef dot_STATIC
#define DOT_STATIC
#endif

#ifdef dot_EXPORTS
#define DOT_EXPORTS
#endif

// доступ к типам для Windows сборки .dll
#if !defined(_WIN32) || defined(DOT_STATIC)
#   define DOT_PUBLIC
#elif defined(DOT_EXPORTS)
#   define DOT_PUBLIC __declspec(dllexport)
#else
#   define DOT_PUBLIC __declspec(dllimport)
#endif

namespace dot
{
    // базовый объект
    class object;

    // базовый класс для всех box
    class box_based;

    // данные класcа помещаются в буфер
    template <class shape>
    class box;

    // базовый класс для всех rope
    class rope_based;

    // данные класса доступны по ссылке
    template <class instance>
    class rope;
}

// Здесь должен быть Unicode
