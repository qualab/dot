// DOT public declaration

#include <dot/type.h>
#include <dot/exception.h>

namespace dot
{
    void invalid_typecast(class_name_type to_type_name, class_name_type from_type_name)
    {
        throw typecast_exception(to_type_name, from_type_name);
    }
}

// Unicode signature: Владимир Керимов
