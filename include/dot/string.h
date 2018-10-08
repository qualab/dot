// DOT copy-on-write object

#pragma once

#include <dot/copyable.h>
#include <string>

namespace dot
{
    template<> DOT_PUBLIC const class_id& copyable<std::string>::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable<std::wstring>::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable<std::u16string>::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable<std::u32string>::id() noexcept;

    template<> DOT_PUBLIC const class_id& copyable<std::string>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable<std::wstring>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable<std::u16string>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& copyable<std::u32string>::data::id() noexcept;

    template<> DOT_PUBLIC void copyable<std::wstring  >::data::write(std::ostream& stream) const;
    template<> DOT_PUBLIC void copyable<std::u16string>::data::write(std::ostream& stream) const;
    template<> DOT_PUBLIC void copyable<std::u32string>::data::write(std::ostream& stream) const;

    template<> DOT_PUBLIC void copyable<std::wstring  >::data::read(std::istream& stream);
    template<> DOT_PUBLIC void copyable<std::u16string>::data::read(std::istream& stream);
    template<> DOT_PUBLIC void copyable<std::u32string>::data::read(std::istream& stream);
}

// Unicode signature: Владимир Керимов
