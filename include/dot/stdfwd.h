// STL forward declaration

#pragma once

namespace std
{
    typedef decltype(nullptr) nullptr_t;

    template <typename char_type>
    class allocator;

    template <typename char_type>
    struct char_traits;

    template <typename char_type, typename traits_type, typename allocator_type>
    class basic_string;

    typedef basic_string<char,     char_traits<char>,     allocator<char>>     string;
    typedef basic_string<wchar_t,  char_traits<wchar_t>,  allocator<wchar_t>>  wstring;
    typedef basic_string<char16_t, char_traits<char16_t>, allocator<char16_t>> u16string;
    typedef basic_string<char32_t, char_traits<char32_t>, allocator<char32_t>> u32string;

    template <typename char_type, typename traits_type>
    class basic_ostream;

    template <typename char_type, typename traits_type>
    class basic_istream;

    typedef basic_ostream<char, char_traits<char>> ostream;
    typedef basic_istream<char, char_traits<char>> istream;

    typedef basic_ostream<wchar_t, char_traits<wchar_t>> wostream;
    typedef basic_istream<wchar_t, char_traits<wchar_t>> wistream;
}

// Unicode signature: Владимир Керимов
