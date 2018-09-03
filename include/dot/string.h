// DOT copy-on-write object

#pragma once

#include <dot/copyable.h>
#include <string>

namespace dot
{
    template<> DOT_PUBLIC const class_id& copyable<std::string>::id() noexcept;

    template<> DOT_PUBLIC const class_id& copyable<std::string>::data::id() noexcept;
}

// Unicode signature: Владимир Керимов
