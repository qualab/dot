// DOT public declaration

#include <dot/type.h>
#include <dot/exception.h>
#include <atomic>

namespace dot
{
    namespace
    {
        std::atomic<uint64> last_class_id;
    }

    class_id::class_id(const char* const name)
        : my_name(name), my_index(++last_class_id)
    {
    }

    const char* const class_id::name() const
    {
        return my_name;
    }

    const uint64 class_id::index() const
    {
        return my_index;
    }

    bool class_id::operator == (const class_id& another) const
    {
        return my_index == another.my_index;
    }

    bool class_id::operator != (const class_id& another) const
    {
        return !(*this == another);
    }

    void invalid_typecast(const class_id& to_class, const class_id& from_class)
    {
        throw typecast_exception(to_class.name(), from_class.name());
    }
}

// Unicode signature: Владимир Керимов
