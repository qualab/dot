// DOT test main

#include <dot/test.h>
#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(scalar_creation)
    {
        scalar<long long> LL = -1234567890123456789LL;
        scalar<long> L = 123456789L;
        scalar<int> i = -987654321;
        scalar<short> s = short(-32100);
        scalar<char> c = 'c';
        scalar<unsigned long long> uLL = 12345678901234567890uLL;
        scalar<unsigned long> uL = 4201234567uL;
        scalar<unsigned int> ui = 0xbadfaced;
        scalar<unsigned short> us = unsigned short(0x1eaf);
        scalar<unsigned char> uc = 'u';
        scalar<double> d = 987.65432109876;
        scalar<float> f = 54.321f;
        scalar<bool> b = true;
        DOT_CHECK(long long(LL)) == -1234567890123456789LL;
        DOT_CHECK(long(L)) == 123456789L;
        DOT_CHECK(int(i)) == -987654321;
        DOT_CHECK(short(s)) == -32100;
        DOT_CHECK(char(c)) == 'c';
        DOT_CHECK(unsigned long long(uLL)) == 12345678901234567890uLL;
        DOT_CHECK(unsigned long(uL)) == 4201234567uL;
        DOT_CHECK(unsigned int(ui)) == 0xbadfaced;
        DOT_CHECK(unsigned short(us)) == 0x1eaf;
        DOT_CHECK(unsigned char(uc)) == 'u';
        DOT_CHECK(double(d)) == 987.65432109876;
        DOT_CHECK(float(f)) == 54.321f;
        DOT_CHECK(bool(b)).is_true();
    }
}

// Unicode signature: Владимир Керимов
