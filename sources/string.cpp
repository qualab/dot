// DOT scalar data object

#include <dot/string.h>

namespace dot
{
    template<> const class_id& copyable<std::string>::id() noexcept
    {
        static const class_id copyable_string_id("copyable<string>");
        return copyable_string_id;
    }

    template<> const class_id& copyable<std::wstring>::id() noexcept
    {
        static const class_id copyable_wstring_id("copyable<wstring>");
        return copyable_wstring_id;
    }

    template<> const class_id& copyable<std::u16string>::id() noexcept
    {
        static const class_id copyable_u16string_id("copyable<u16string>");
        return copyable_u16string_id;
    }

    template<> const class_id& copyable<std::u32string>::id() noexcept
    {
        static const class_id copyable_u32string_id("copyable<u32string>");
        return copyable_u32string_id;
    }

    template<> const class_id& copyable<std::string>::data::id() noexcept
    {
        static const class_id copyable_string_data_id("copyable<string>::data");
        return copyable_string_data_id;
    }

    template<> const class_id& copyable<std::wstring>::data::id() noexcept
    {
        static const class_id copyable_wstring_data_id("copyable<wstring>::data");
        return copyable_wstring_data_id;
    }

    template<> const class_id& copyable<std::u16string>::data::id() noexcept
    {
        static const class_id copyable_u16string_data_id("copyable<u16string>::data");
        return copyable_u16string_data_id;
    }

    template<> const class_id& copyable<std::u32string>::data::id() noexcept
    {
        static const class_id copyable_u32string_data_id("copyable<u32string>::data");
        return copyable_u32string_data_id;
    }

    template<> void copyable<std::wstring  >::data::write(std::ostream& stream) const { stream; /* TODO: cross-platform output widechar string to byte stream */ }
    template<> void copyable<std::u16string>::data::write(std::ostream& stream) const { stream; /* TODO: cross-platform output 16-bit char string to byte stream */ }
    template<> void copyable<std::u32string>::data::write(std::ostream& stream) const { stream; /* TODO: cross-platform output 32-bit char string to byte stream */ }

    template<> void copyable<std::wstring  >::data::read(std::istream& stream) { stream; /* TODO: cross-platform input widechar string from byte stream */ }
    template<> void copyable<std::u16string>::data::read(std::istream& stream) { stream; /* TODO: cross-platform input 16-bit char string from byte stream */ }
    template<> void copyable<std::u32string>::data::read(std::istream& stream) { stream; /* TODO: cross-platform input 32-bit char string from byte stream */ }

    template<> void object::set_as(const char* const& value) { initialize<copyable<std::string>::data>(std::string(value)); }
    template<> void object::set_as(const std::string& value) { initialize<copyable<std::string>::data>(value); }
    template<> void object::set_as(std::string&& value) { initialize<copyable<std::string>::data>(std::move(value)); }

    template<> void object::set_as(const wchar_t* const& value) { initialize<copyable<std::wstring>::data>(std::wstring(value)); }
    template<> void object::set_as(const std::wstring& value) { initialize<copyable<std::wstring>::data>(value); }
    template<> void object::set_as(std::wstring&& value) { initialize<copyable<std::wstring>::data>(std::move(value)); }

    template<> void object::set_as(const char16_t* const& value) { initialize<copyable<std::u16string>::data>(std::u16string(value)); }
    template<> void object::set_as(const std::u16string& value) { initialize<copyable<std::u16string>::data>(value); }
    template<> void object::set_as(std::u16string&& value) { initialize<copyable<std::u16string>::data>(std::move(value)); }

    template<> void object::set_as(const char32_t* const& value) { initialize<copyable<std::u32string>::data>(std::u32string(value)); }
    template<> void object::set_as(const std::u32string& value) { initialize<copyable<std::u32string>::data>(value); }
    template<> void object::set_as(std::u32string&& value) { initialize<copyable<std::u32string>::data>(std::move(value)); }

    template<> std::string    object::get_as() const { return data_as<copyable<std::string>::data>().get(); }
    template<> std::wstring   object::get_as() const { return data_as<copyable<std::wstring>::data>().get(); }
    template<> std::u16string object::get_as() const { return data_as<copyable<std::u16string>::data>().get(); }
    template<> std::u32string object::get_as() const { return data_as<copyable<std::u32string>::data>().get(); }

    template<> const std::string&    object::get_as() const { return data_as<copyable<std::string>::data>().get(); }
    template<> const std::wstring&   object::get_as() const { return data_as<copyable<std::wstring>::data>().get(); }
    template<> const std::u16string& object::get_as() const { return data_as<copyable<std::u16string>::data>().get(); }
    template<> const std::u32string& object::get_as() const { return data_as<copyable<std::u32string>::data>().get(); }

    template<> const char*     object::get_as() const { return data_as<copyable<std::string>::data>().get().c_str(); }
    template<> const wchar_t*  object::get_as() const { return data_as<copyable<std::wstring>::data>().get().c_str(); }
    template<> const char16_t* object::get_as() const { return data_as<copyable<std::u16string>::data>().get().c_str(); }
    template<> const char32_t* object::get_as() const { return data_as<copyable<std::u32string>::data>().get().c_str(); }
}

// Unicode signature: Владимир Керимов
