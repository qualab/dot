// DOT test main

#include <dot/test.h>
#include <dot/object.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(object_null)
    {
        object obj;
        DOT_CHECK(obj).is_null();
        DOT_CHECK(obj).is<object>();
    }

    DOT_TEST_SUITE(object_set)
    {
        object i = 65536;
        object u = 1234567890123456789uLL;
        object d = 2.345678901e+123;
        object f = 3.45678e+12f;
        object b = true;
        DOT_CHECK(i.get_as<int>()) == 65536;
        DOT_CHECK(u.get_as<uint64>()) == 1234567890123456789uLL;
        DOT_CHECK(d.get_as<double>()) == 2.345678901e+123;
        DOT_CHECK(f.get_as<float>()) == 3.45678e+12f;
        DOT_CHECK(b.get_as<bool>()).is_true();
    }
}

// Unicode signature: Владимир Керимов
