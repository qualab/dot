// DOT scalar data object

#include <dot/string.h>

namespace dot
{
    template<> const class_id& copyable<std::string>::id() noexcept
    {
        static const class_id copyable_string_id("copyable<string>");
        return copyable_string_id;
    }

    template<> const class_id& copyable<std::string>::data::id() noexcept
    {
        static const class_id copyable_string_data_id("copyable<string>::data");
        return copyable_string_data_id;
    }

    template<> void copyable<std::wstring  >::data::write(std::ostream& stream) const { stream; /* TODO: cross-platform output widechar string to byte stream */ }
    template<> void copyable<std::u16string>::data::write(std::ostream& stream) const { stream; /* TODO: cross-platform output 16-bit char string to byte stream */ }
    template<> void copyable<std::u32string>::data::write(std::ostream& stream) const { stream; /* TODO: cross-platform output 32-bit char string to byte stream */ }

    template<> void copyable<std::wstring  >::data::read(std::istream& stream) { stream; /* TODO: cross-platform input widechar string from byte stream */ }
    template<> void copyable<std::u16string>::data::read(std::istream& stream) { stream; /* TODO: cross-platform input 16-bit char string from byte stream */ }
    template<> void copyable<std::u32string>::data::read(std::istream& stream) { stream; /* TODO: cross-platform input 32-bit char string from byte stream */ }
}

// Unicode signature: Владимир Керимов
