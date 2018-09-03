// DOT test main

#include <dot/test.h>
#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(scalar_creation)
    {
        scalar<int> i = 5;
        DOT_CHECK(i.get_as<int>()) == 5;
    }
}

// Unicode signature: Владимир Керимов
