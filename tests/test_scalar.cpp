// DOT test main

#include <dot/test.h>
#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(scalar_creation)
    {
        scalar<int> i = 123456789;
        scalar<bool> b = true;
        scalar<long long> L = 1234567890123456789LL;
        scalar<unsigned short> s = unsigned short(0x1eaf);
        scalar<unsigned long> u = 4201234567uL;
        DOT_CHECK(int(i)) == 123456789;
        DOT_CHECK(bool(b)).is_true();
        DOT_CHECK(long long(L)) == 1234567890123456789LL;
        DOT_CHECK(unsigned short(s)) == unsigned short(0x1eaf);
        DOT_CHECK(unsigned long(u)) == 4201234567uL;
    }
}

// Unicode signature: Владимир Керимов
