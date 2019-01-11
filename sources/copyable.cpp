// DOT copyable data object

#include <dot/copyable.h>

namespace dot
{
    copyable::copyable()
    {
    }

    const class_id& copyable::id() noexcept
    {
        static const class_id copyable_id("copyable");
        return copyable_id;
    }

    const class_id& copyable::data::id() noexcept
    {
        static const class_id copyable_data_id("copyable::data");
        return copyable_data_id;
    }
}

// Unicode signature: Владимир Керимов
