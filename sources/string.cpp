// dot::rope which are bound to the literal cows
// implementation for the copy-on-write strings

#include <dot/string.h>

namespace dot
{
    using std::string;
    using std::wstring;
    using std::u16string;
    using std::u32string;

    template<> DOT_CLASS_ID(rope<string>)
    template<> DOT_CLASS_ID(rope<wstring>)
    template<> DOT_CLASS_ID(rope<u16string>)
    template<> DOT_CLASS_ID(rope<u32string>)

    template<> DOT_CLASS_ID(rope<string>::cow)
    template<> DOT_CLASS_ID(rope<wstring>::cow)
    template<> DOT_CLASS_ID(rope<u16string>::cow)
    template<> DOT_CLASS_ID(rope<u32string>::cow)

    template<> void object::set_as(const char* const& value) { initialize<rope<string>::cow>(string(value)); }
    template<> void object::set_as(const string& value) { initialize<rope<string>::cow>(value); }
    template<> void object::set_as(string&& value) { initialize<rope<string>::cow>(move(value)); }

    template<> void object::set_as(const wchar_t* const& value) { initialize<rope<wstring>::cow>(wstring(value)); }
    template<> void object::set_as(const wstring& value) { initialize<rope<wstring>::cow>(value); }
    template<> void object::set_as(wstring&& value) { initialize<rope<wstring>::cow>(move(value)); }

    template<> void object::set_as(const char16_t* const& value) { initialize<rope<u16string>::cow>(u16string(value)); }
    template<> void object::set_as(const u16string& value) { initialize<rope<u16string>::cow>(value); }
    template<> void object::set_as(u16string&& value) { initialize<rope<u16string>::cow>(move(value)); }

    template<> void object::set_as(const char32_t* const& value) { initialize<rope<u32string>::cow>(u32string(value)); }
    template<> void object::set_as(const u32string& value) { initialize<rope<u32string>::cow>(value); }
    template<> void object::set_as(u32string&& value) { initialize<rope<u32string>::cow>(move(value)); }

    template<> string    object::get_as() const { return data_as<rope<string>::cow>().look(); }
    template<> wstring   object::get_as() const { return data_as<rope<wstring>::cow>().look(); }
    template<> u16string object::get_as() const { return data_as<rope<u16string>::cow>().look(); }
    template<> u32string object::get_as() const { return data_as<rope<u32string>::cow>().look(); }

    template<> const string&    object::get_as() const { return data_as<rope<string>::cow>().look(); }
    template<> const wstring&   object::get_as() const { return data_as<rope<wstring>::cow>().look(); }
    template<> const u16string& object::get_as() const { return data_as<rope<u16string>::cow>().look(); }
    template<> const u32string& object::get_as() const { return data_as<rope<u32string>::cow>().look(); }

    template<> const char*     object::get_as() const { return data_as<rope<string>::cow>().look().c_str(); }
    template<> const wchar_t*  object::get_as() const { return data_as<rope<wstring>::cow>().look().c_str(); }
    template<> const char16_t* object::get_as() const { return data_as<rope<u16string>::cow>().look().c_str(); }
    template<> const char32_t* object::get_as() const { return data_as<rope<u32string>::cow>().look().c_str(); }
}

// Здесь должен быть Unicode
