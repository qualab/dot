// dot::rope which are bound to the literal cows
// implementation for the copy-on-write strings

#include <dot/string.h>

namespace dot
{
    template<> const class_id& rope<std::string>::id() noexcept
    {
        static const class_id rope_string_id("rope<string>");
        return rope_string_id;
    }

    template<> const class_id& rope<std::wstring>::id() noexcept
    {
        static const class_id rope_wstring_id("rope<wstring>");
        return rope_wstring_id;
    }

    template<> const class_id& rope<std::u16string>::id() noexcept
    {
        static const class_id rope_u16string_id("rope<u16string>");
        return rope_u16string_id;
    }

    template<> const class_id& rope<std::u32string>::id() noexcept
    {
        static const class_id rope_u32string_id("rope<u32string>");
        return rope_u32string_id;
    }

    template<> const class_id& rope<std::string>::cow::id() noexcept
    {
        static const class_id rope_string_cow_id("rope<string>::cow");
        return rope_string_cow_id;
    }

    template<> const class_id& rope<std::wstring>::cow::id() noexcept
    {
        static const class_id rope_wstring_cow_id("rope<wstring>::cow");
        return rope_wstring_cow_id;
    }

    template<> const class_id& rope<std::u16string>::cow::id() noexcept
    {
        static const class_id rope_u16string_cow_id("rope<u16string>::cow");
        return rope_u16string_cow_id;
    }

    template<> const class_id& rope<std::u32string>::cow::id() noexcept
    {
        static const class_id rope_u32string_cow_id("rope<u32string>::cow");
        return rope_u32string_cow_id;
    }

    template<> void object::set_as(const char* const& value) { initialize<rope<std::string>::cow>(std::string(value)); }
    template<> void object::set_as(const std::string& value) { initialize<rope<std::string>::cow>(value); }
    template<> void object::set_as(std::string&& value) { initialize<rope<std::string>::cow>(std::move(value)); }

    template<> void object::set_as(const wchar_t* const& value) { initialize<rope<std::wstring>::cow>(std::wstring(value)); }
    template<> void object::set_as(const std::wstring& value) { initialize<rope<std::wstring>::cow>(value); }
    template<> void object::set_as(std::wstring&& value) { initialize<rope<std::wstring>::cow>(std::move(value)); }

    template<> void object::set_as(const char16_t* const& value) { initialize<rope<std::u16string>::cow>(std::u16string(value)); }
    template<> void object::set_as(const std::u16string& value) { initialize<rope<std::u16string>::cow>(value); }
    template<> void object::set_as(std::u16string&& value) { initialize<rope<std::u16string>::cow>(std::move(value)); }

    template<> void object::set_as(const char32_t* const& value) { initialize<rope<std::u32string>::cow>(std::u32string(value)); }
    template<> void object::set_as(const std::u32string& value) { initialize<rope<std::u32string>::cow>(value); }
    template<> void object::set_as(std::u32string&& value) { initialize<rope<std::u32string>::cow>(std::move(value)); }

    template<> std::string    object::get_as() const { return data_as<rope<std::string>::cow>().look(); }
    template<> std::wstring   object::get_as() const { return data_as<rope<std::wstring>::cow>().look(); }
    template<> std::u16string object::get_as() const { return data_as<rope<std::u16string>::cow>().look(); }
    template<> std::u32string object::get_as() const { return data_as<rope<std::u32string>::cow>().look(); }

    template<> const std::string&    object::get_as() const { return data_as<rope<std::string>::cow>().look(); }
    template<> const std::wstring&   object::get_as() const { return data_as<rope<std::wstring>::cow>().look(); }
    template<> const std::u16string& object::get_as() const { return data_as<rope<std::u16string>::cow>().look(); }
    template<> const std::u32string& object::get_as() const { return data_as<rope<std::u32string>::cow>().look(); }

    template<> const char*     object::get_as() const { return data_as<rope<std::string>::cow>().look().c_str(); }
    template<> const wchar_t*  object::get_as() const { return data_as<rope<std::wstring>::cow>().look().c_str(); }
    template<> const char16_t* object::get_as() const { return data_as<rope<std::u16string>::cow>().look().c_str(); }
    template<> const char32_t* object::get_as() const { return data_as<rope<std::u32string>::cow>().look().c_str(); }
}

// Здесь должен быть Unicode
