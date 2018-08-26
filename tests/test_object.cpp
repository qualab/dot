// DOT test main

#include <dot/test.h>
#include <dot/object.h>

namespace dot
{
    DOT_TEST_SUITE(object_null)
    {
        object obj;
        DOT_TEST_CHECK(obj).is_null();
    }
}

// Unicode signature: Владимир Керимов
