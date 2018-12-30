// DOT scalar data object

#include <dot/string.h>

namespace dot
{
    template<> const class_id& copyable_of<std::string>::id() noexcept
    {
        static const class_id copyable_of_string_id("copyable_of<string>");
        return copyable_of_string_id;
    }

    template<> const class_id& copyable_of<std::wstring>::id() noexcept
    {
        static const class_id copyable_of_wstring_id("copyable_of<wstring>");
        return copyable_of_wstring_id;
    }

    template<> const class_id& copyable_of<std::u16string>::id() noexcept
    {
        static const class_id copyable_of_u16string_id("copyable_of<u16string>");
        return copyable_of_u16string_id;
    }

    template<> const class_id& copyable_of<std::u32string>::id() noexcept
    {
        static const class_id copyable_of_u32string_id("copyable_of<u32string>");
        return copyable_of_u32string_id;
    }

    template<> const class_id& copyable_of<std::string>::data::id() noexcept
    {
        static const class_id copyable_of_string_data_id("copyable_of<string>::data");
        return copyable_of_string_data_id;
    }

    template<> const class_id& copyable_of<std::wstring>::data::id() noexcept
    {
        static const class_id copyable_of_wstring_data_id("copyable_of<wstring>::data");
        return copyable_of_wstring_data_id;
    }

    template<> const class_id& copyable_of<std::u16string>::data::id() noexcept
    {
        static const class_id copyable_of_u16string_data_id("copyable_of<u16string>::data");
        return copyable_of_u16string_data_id;
    }

    template<> const class_id& copyable_of<std::u32string>::data::id() noexcept
    {
        static const class_id copyable_of_u32string_data_id("copyable_of<u32string>::data");
        return copyable_of_u32string_data_id;
    }

    template<> void object::set_as(const char* const& value) { initialize<copyable_of<std::string>::data>(std::string(value)); }
    template<> void object::set_as(const std::string& value) { initialize<copyable_of<std::string>::data>(value); }
    template<> void object::set_as(std::string&& value) { initialize<copyable_of<std::string>::data>(std::move(value)); }

    template<> void object::set_as(const wchar_t* const& value) { initialize<copyable_of<std::wstring>::data>(std::wstring(value)); }
    template<> void object::set_as(const std::wstring& value) { initialize<copyable_of<std::wstring>::data>(value); }
    template<> void object::set_as(std::wstring&& value) { initialize<copyable_of<std::wstring>::data>(std::move(value)); }

    template<> void object::set_as(const char16_t* const& value) { initialize<copyable_of<std::u16string>::data>(std::u16string(value)); }
    template<> void object::set_as(const std::u16string& value) { initialize<copyable_of<std::u16string>::data>(value); }
    template<> void object::set_as(std::u16string&& value) { initialize<copyable_of<std::u16string>::data>(std::move(value)); }

    template<> void object::set_as(const char32_t* const& value) { initialize<copyable_of<std::u32string>::data>(std::u32string(value)); }
    template<> void object::set_as(const std::u32string& value) { initialize<copyable_of<std::u32string>::data>(value); }
    template<> void object::set_as(std::u32string&& value) { initialize<copyable_of<std::u32string>::data>(std::move(value)); }

    template<> std::string    object::get_as() const { return data_as<copyable_of<std::string>::data>().get(); }
    template<> std::wstring   object::get_as() const { return data_as<copyable_of<std::wstring>::data>().get(); }
    template<> std::u16string object::get_as() const { return data_as<copyable_of<std::u16string>::data>().get(); }
    template<> std::u32string object::get_as() const { return data_as<copyable_of<std::u32string>::data>().get(); }

    template<> const std::string&    object::get_as() const { return data_as<copyable_of<std::string>::data>().get(); }
    template<> const std::wstring&   object::get_as() const { return data_as<copyable_of<std::wstring>::data>().get(); }
    template<> const std::u16string& object::get_as() const { return data_as<copyable_of<std::u16string>::data>().get(); }
    template<> const std::u32string& object::get_as() const { return data_as<copyable_of<std::u32string>::data>().get(); }

    template<> const char*     object::get_as() const { return data_as<copyable_of<std::string>::data>().get().c_str(); }
    template<> const wchar_t*  object::get_as() const { return data_as<copyable_of<std::wstring>::data>().get().c_str(); }
    template<> const char16_t* object::get_as() const { return data_as<copyable_of<std::u16string>::data>().get().c_str(); }
    template<> const char32_t* object::get_as() const { return data_as<copyable_of<std::u32string>::data>().get().c_str(); }
}

// Unicode signature: Владимир Керимов
