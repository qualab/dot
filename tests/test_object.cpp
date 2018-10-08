// DOT test main

#include <dot/test.h>
#include <dot/object.h>
#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(object_null)
    {
        object obj;
        DOT_CHECK(obj).is_null();
        DOT_CHECK(obj).is<object>();
        DOT_CHECK(obj.is_null()).is_true();
        DOT_CHECK(obj.is_not_null()).is_false();

        object nil = obj;
        DOT_CHECK(nil).is_null();
        DOT_CHECK(nil).is<object>();
        DOT_CHECK(nil.is_null()).is_true();
        DOT_CHECK(nil.is_not_null()).is_false();
    }

    DOT_TEST_SUITE(object_set)
    {
        object LL = 9012345678901234567LL;
        object L = -2109876543L;
        object i = -1;
        object s = short(0x1ead);
        object c = '%';
        object uLL = 12345678909876543210uLL;
        object uL = 0xdeadceeduL;
        object u = 1024u;
        object us = unsigned short(0xfee1);
        object uc = unsigned char('~');
        object d = 2.345678901e+123;
        object f = 3.45678e+12f;
        object b = true;

        DOT_CHECK(LL.get_as<long long>()) == 9012345678901234567LL;
        DOT_CHECK(L.get_as<long>()) == -2109876543L;
        DOT_CHECK(i.get_as<int>()) == -1;
        DOT_CHECK(s.get_as<short>()) == short(0x1ead);
        DOT_CHECK(c.get_as<char>()) == '%';
        DOT_CHECK(uLL.get_as<unsigned long long>()) == 12345678909876543210uLL;
        DOT_CHECK(uL.get_as<unsigned long>()) == 0xdeadceeduL;
        DOT_CHECK(u.get_as<unsigned int>()) == 1024u;
        DOT_CHECK(us.get_as<unsigned short>()) == unsigned short(0xfee1);
        DOT_CHECK(uc.get_as<unsigned char>()) == unsigned char('~');
        DOT_CHECK(d.get_as<double>()) == 2.345678901e+123;
        DOT_CHECK(f.get_as<float>()) == 3.45678e+12f;
        DOT_CHECK(b).is_true();
    }
}

// Unicode signature: Владимир Керимов
