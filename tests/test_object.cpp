// DOT test main

#include <dot/test.h>
#include <dot/object.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(object_null)
    {
        object obj;
        DOT_TEST_CHECK(obj).is_null();
        DOT_TEST_CHECK(obj).is<object>();
    }
}

// Unicode signature: Владимир Керимов
