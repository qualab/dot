// DOT test main

#include <dot/test.h>
#include <dot/scalar.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(scalar_class_hierarchy)
    {
        DOT_CHECK(is_class<scalar>::of<object>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>>::of<object>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>>::of<scalar>()).is_true();
        DOT_CHECK(is_class<scalar_of<uint>>::of<object>()).is_true();
        DOT_CHECK(is_class<scalar_of<uint>>::of<scalar>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>>::of<scalar_of<uint>>()).is_false();
        DOT_CHECK(is_class<scalar_of<uint>>::of<scalar_of<int>>()).is_false();
        DOT_CHECK(is_class<object>::of<scalar_of<int>>()).is_false();
        DOT_CHECK(is_class<object>::of<scalar_of<uint>>()).is_false();
        DOT_CHECK(is_class<scalar>::of<scalar_of<int>>()).is_false();
        DOT_CHECK(is_class<scalar>::of<scalar_of<uint>>()).is_false();
        DOT_CHECK(is_class<object>::of<scalar>()).is_false();
        DOT_CHECK(is_class<scalar>::of<scalar>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>>::of<scalar_of<int>>()).is_true();
        DOT_CHECK(is_class<scalar_of<uint>>::of<scalar_of<uint>>()).is_true();

        DOT_CHECK(is_class<scalar::data>::of<object::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>::data>::of<object::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>::data>::of<scalar::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<uint>::data>::of<object::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<uint>::data>::of<scalar::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>::data>::of<scalar_of<uint>::data>()).is_false();
        DOT_CHECK(is_class<scalar_of<uint>::data>::of<scalar_of<int>::data>()).is_false();
        DOT_CHECK(is_class<object::data>::of<scalar_of<int>::data>()).is_false();
        DOT_CHECK(is_class<object::data>::of<scalar_of<uint>::data>()).is_false();
        DOT_CHECK(is_class<scalar::data>::of<scalar_of<int>::data>()).is_false();
        DOT_CHECK(is_class<scalar::data>::of<scalar_of<uint>::data>()).is_false();
        DOT_CHECK(is_class<object::data>::of<scalar::data>()).is_false();
        DOT_CHECK(is_class<scalar::data>::of<scalar::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<int>::data>::of<scalar_of<int>::data>()).is_true();
        DOT_CHECK(is_class<scalar_of<uint>::data>::of<scalar_of<uint>::data>()).is_true();
    }

    DOT_TEST_SUITE(scalar_instance_hierarchy)
    {
        scalar n;
        DOT_CHECK(n).is_null();
        DOT_CHECK(n).is<scalar>();
        DOT_CHECK(n).is<object>();
        DOT_CHECK(n).is_not<scalar_of<int>>();
        DOT_CHECK(n.who() == scalar::id()).is_true();
        DOT_CHECK(n.who() == scalar_of<int>::id()).is_false();
        DOT_CHECK(n.who() == object::id()).is_false();

        scalar i(-1);
        DOT_CHECK(i).is_not_null();
        DOT_CHECK(scalar_of<int>(i)) == -1;
        DOT_CHECK(i).is<scalar>();
        DOT_CHECK(i).is<object>();
        DOT_CHECK(i).is_not<scalar_of<int>>();
        DOT_CHECK(i.get_data()).is<scalar_of<int>::data>();
        DOT_CHECK(i.get_data()).is<scalar::data>();
        DOT_CHECK(i.get_data()).is_not<scalar_of<uint>::data>();
        DOT_CHECK(i.get_data()).is<object::data>();
        DOT_CHECK(i.who() == scalar::id()).is_true();
        DOT_CHECK(i.who() == scalar_of<int>::id()).is_false();
        DOT_CHECK(i.who() == object::id()).is_false();

        scalar_of<uint> u(0x1dea0fu);
        DOT_CHECK(u).is_not_null();
        DOT_CHECK(u) == 0x1dea0fu;
        DOT_CHECK(u).is<scalar_of<uint>>();
        DOT_CHECK(u).is_not<scalar_of<int>>();
        DOT_CHECK(u).is<scalar>();
        DOT_CHECK(u).is<object>();
        DOT_CHECK(u.get_data()).is<scalar_of<uint>::data>();
        DOT_CHECK(u.get_data()).is<scalar::data>();
        DOT_CHECK(u.get_data()).is_not<scalar_of<int>::data>();
        DOT_CHECK(u.get_data()).is<object::data>();

        scalar_of<int> t(1234567890);
        DOT_CHECK(t).is_not_null();
        DOT_CHECK(t) == 1234567890;
        DOT_CHECK(t).is<scalar_of<int>>();
        DOT_CHECK(t).is_not<scalar_of<uint>>();
        DOT_CHECK(t).is<scalar>();
        DOT_CHECK(t).is<object>();
        DOT_CHECK(t.get_data()).is<scalar_of<int>::data>();
        DOT_CHECK(t.get_data()).is<scalar::data>();
        DOT_CHECK(t.get_data()).is_not<scalar_of<uint>::data>();
        DOT_CHECK(t.get_data()).is<object::data>();
    }

    DOT_TEST_SUITE(scalar_creation)
    {
        scalar_of<long long> LL(-1234567890123456789LL);
        scalar_of<long> L(123456789L);
        scalar_of<int> i(-987654321);
        scalar_of<short> s(short(-32100));
        scalar_of<char> c('c');
        scalar_of<unsigned long long> uLL(12345678901234567890uLL);
        scalar_of<unsigned long> uL(4201234567uL);
        scalar_of<unsigned int> ui(0xbadfaced);
        scalar_of<unsigned short> us(unsigned short(0x1eaf));
        scalar_of<unsigned char> uc(unsigned char('u'));
        scalar_of<double> d(987.65432109876);
        scalar_of<float> f(54.321f);
        scalar_of<bool> b(true);

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

        object oLL = LL;
        object oL = L;
        object oi = i;
        object os = s;
        object oc = c;
        object ouLL = uLL;
        object ouL = uL;
        object oui = ui;
        object ous = us;
        object ouc = uc;
        object od = d;
        object of = f;
        object ob = b;

        DOT_CHECK(long long(oLL)) == -1234567890123456789LL;
        DOT_CHECK(long(oL)) == 123456789L;
        DOT_CHECK(int(oi)) == -987654321;
        DOT_CHECK(short(os)) == -32100;
        DOT_CHECK(char(oc)) == 'c';
        DOT_CHECK(unsigned long long(ouLL)) == 12345678901234567890uLL;
        DOT_CHECK(unsigned long(ouL)) == 4201234567uL;
        DOT_CHECK(unsigned int(oui)) == 0xbadfaced;
        DOT_CHECK(unsigned short(ous)) == 0x1eaf;
        DOT_CHECK(unsigned char(ouc)) == 'u';
        DOT_CHECK(double(od)) == 987.65432109876;
        DOT_CHECK(float(of)) == 54.321f;
        DOT_CHECK(bool(ob)).is_true();

        scalar_of<long long> aLL = oLL;
        scalar_of<long> aL = oL;
        scalar_of<int> ai = oi;
        scalar_of<short> as = os;
        scalar_of<char> ac = oc;
        scalar_of<unsigned long long> auLL = ouLL;
        scalar_of<unsigned long> auL = ouL;
        scalar_of<unsigned int> aui = oui;
        scalar_of<unsigned short> aus = ous;
        scalar_of<unsigned char> auc = ouc;
        scalar_of<double> ad = od;
        scalar_of<float> af = of;
        scalar_of<bool> ab = ob;

        DOT_CHECK(long long(aLL)) == -1234567890123456789LL;
        DOT_CHECK(long(aL)) == 123456789L;
        DOT_CHECK(int(ai)) == -987654321;
        DOT_CHECK(short(as)) == -32100;
        DOT_CHECK(char(ac)) == 'c';
        DOT_CHECK(unsigned long long(auLL)) == 12345678901234567890uLL;
        DOT_CHECK(unsigned long(auL)) == 4201234567uL;
        DOT_CHECK(unsigned int(aui)) == 0xbadfaced;
        DOT_CHECK(unsigned short(aus)) == 0x1eaf;
        DOT_CHECK(unsigned char(auc)) == 'u';
        DOT_CHECK(double(ad)) == 987.65432109876;
        DOT_CHECK(float(af)) == 54.321f;
        DOT_CHECK(bool(ab)).is_true();

        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, ab = c);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, af = d);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, ad = i);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, ac = s);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, as = i);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, ai = L);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, aL = LL);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, aLL = uLL);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, auc = us);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, aus = ui);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, aui = uL);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, auL = uLL);
        DOT_CHECK_EXPECT_EXCEPTION(fail::bad_typecast, auLL = LL);

        DOT_CHECK(LL) == aLL;
        DOT_CHECK(L) == aL;
        DOT_CHECK(i) == ai;
        DOT_CHECK(s) == as;
        DOT_CHECK(c) == ac;
        DOT_CHECK(uLL) == auLL;
        DOT_CHECK(uL) == auL;
        DOT_CHECK(ui) == aui;
        DOT_CHECK(us) == aus;
        DOT_CHECK(uc) == auc;
        DOT_CHECK(d) == ad;
        DOT_CHECK(f) == af;
        DOT_CHECK(b) == ab;
    }

    namespace
    {
        struct test_type
        {
            uint64 index;
            double value;

            test_type(uint64 id, double num)
                : index(id), value(num) { }
        };
    }

    template<> const class_id& scalar_of<test_type>::id() noexcept
    {
        static const class_id scalar_test_type_id("scalar_of<test_type>");
        return scalar_test_type_id;
    }

    template<> const class_id& scalar_of<test_type>::data::id() noexcept
    {
        static const class_id scalar_test_type_data_id("scalar_of<test_type>::data");
        return scalar_test_type_data_id;
    }

    DOT_TEST_SUITE(scalar_test_type)
    {
        object x(test_type(100500uLL, 9000.1));
        DOT_ENSURE(x.get_data()).is<scalar_of<test_type>::data>();
        DOT_ENSURE_NO_EXCEPTION(x.get_as<test_type>());
        DOT_CHECK(x.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(x.get_as<test_type>().value) == 9000.1;

        scalar_of<test_type> y = x;
        DOT_ENSURE(y).is<scalar_of<test_type>>();
        DOT_ENSURE(y.get_data()).is<scalar_of<test_type>::data>();
        DOT_ENSURE_NO_EXCEPTION(y.get_as<test_type>());
        DOT_CHECK(y.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(y.get_as<test_type>().value) == 9000.1;

        DOT_ENSURE_NO_EXCEPTION(
            x = test_type(12345678901234567890uLL, -1234567.87654321));
        DOT_ENSURE(x.get_data()).is<scalar_of<test_type>::data>();
        DOT_ENSURE_NO_EXCEPTION(x.get_as<test_type>());
        DOT_CHECK(x.get_as<test_type>().index) == 12345678901234567890uLL;
        DOT_CHECK(x.get_as<test_type>().value) == -1234567.87654321;

        DOT_ENSURE(y).is<scalar_of<test_type>>();
        DOT_ENSURE(y.get_data()).is<scalar_of<test_type>::data>();
        DOT_ENSURE_NO_EXCEPTION(y.get_as<test_type>());
        DOT_CHECK(y.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(y.get_as<test_type>().value) == 9000.1;

        object z = y;
        DOT_ENSURE(z.get_data()).is<scalar_of<test_type>::data>();
        DOT_ENSURE_NO_EXCEPTION(z.get_as<test_type>());
        DOT_CHECK(z.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(z.get_as<test_type>().value) == 9000.1;

        DOT_ENSURE_NO_EXCEPTION(z = 0);
        DOT_ENSURE(z.get_data()).is<scalar_of<int>::data>();
        DOT_CHECK(z.get_as<int>()) == 0;

        DOT_ENSURE(y).is<scalar_of<test_type>>();
        DOT_ENSURE(y.get_data()).is<scalar_of<test_type>::data>();
        DOT_CHECK(y.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(y.get_as<test_type>().value) == 9000.1;

        DOT_ENSURE(x.get_data()).is<scalar_of<test_type>::data>();
        DOT_ENSURE_NO_EXCEPTION(x.get_as<test_type>());
        DOT_CHECK(x.get_as<test_type>().index) == 12345678901234567890uLL;
        DOT_CHECK(x.get_as<test_type>().value) == -1234567.87654321;
    }
}

// Здесь должен быть Unicode
