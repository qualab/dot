// DOT copy-on-write object

#pragma once

#include <dot/copyable.h>
#include <string>

namespace dot
{
    template<> DOT_PUBLIC const class_id& copyable_of<std::string>::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable_of<std::wstring>::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable_of<std::u16string>::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable_of<std::u32string>::id() noexcept;

    template<> DOT_PUBLIC const class_id& copyable_of<std::string>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable_of<std::wstring>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable_of<std::u16string>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable_of<std::u32string>::data::id() noexcept;

    template<> DOT_PUBLIC void object::set_as(const char* const& value);
    template<> DOT_PUBLIC void object::set_as(const std::string& value);
    template<> DOT_PUBLIC void object::set_as(std::string&& value);

    template<> DOT_PUBLIC void object::set_as(const wchar_t* const& value);
    template<> DOT_PUBLIC void object::set_as(const std::wstring& value);
    template<> DOT_PUBLIC void object::set_as(std::wstring&& value);

    template<> DOT_PUBLIC void object::set_as(const char16_t* const& value);
    template<> DOT_PUBLIC void object::set_as(const std::u16string& value);
    template<> DOT_PUBLIC void object::set_as(std::u16string&& value);

    template<> DOT_PUBLIC void object::set_as(const char32_t* const& value);
    template<> DOT_PUBLIC void object::set_as(const std::u32string& value);
    template<> DOT_PUBLIC void object::set_as(std::u32string&& value);

    template<> DOT_PUBLIC std::string    object::get_as() const;
    template<> DOT_PUBLIC std::wstring   object::get_as() const;
    template<> DOT_PUBLIC std::u16string object::get_as() const;
    template<> DOT_PUBLIC std::u32string object::get_as() const;

    template<> DOT_PUBLIC const std::string&    object::get_as() const;
    template<> DOT_PUBLIC const std::wstring&   object::get_as() const;
    template<> DOT_PUBLIC const std::u16string& object::get_as() const;
    template<> DOT_PUBLIC const std::u32string& object::get_as() const;

    template<> DOT_PUBLIC const char*     object::get_as() const;
    template<> DOT_PUBLIC const wchar_t*  object::get_as() const;
    template<> DOT_PUBLIC const char16_t* object::get_as() const;
    template<> DOT_PUBLIC const char32_t* object::get_as() const;
}

// Unicode signature: Владимир Керимов
