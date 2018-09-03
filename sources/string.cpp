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
}

// Unicode signature: Владимир Керимов
