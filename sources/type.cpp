// DOT public declaration

#include <dot/type.h>
#include <dot/fail.h>
#include <atomic>

namespace dot
{
    namespace
    {
        std::atomic<uint64> last_class_id;
    }

    class_id::class_id(const char* const name) noexcept
        : my_name(name), my_index(++last_class_id)
    {
    }

    const char* const class_id::name() const noexcept
    {
        return my_name;
    }

    const uint64 class_id::index() const noexcept
    {
        return my_index;
    }

    bool class_id::operator == (const class_id& another) const noexcept
    {
        return my_index == another.my_index;
    }

    bool class_id::operator != (const class_id& another) const noexcept
    {
        return my_index != another.my_index;
    }

    void invalid_typecast(const class_id& to_class, const class_id& from_class)
    {
        throw fail::bad_typecast(to_class, from_class);
    }
}

// Unicode signature: Владимир Керимов
