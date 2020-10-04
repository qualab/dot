// DOT test main

#include <dot/test.h>
#include <dot/box.h>
#include <iostream>

namespace dot
{
    DOT_TEST_SUITE(box_class_hierarchy)
    {
        DOT_CHECK(is_class<box_based>::of<object>()).is_true();
        DOT_CHECK(is_class<box<int>>::of<object>()).is_true();
        DOT_CHECK(is_class<box<int>>::of<box_based>()).is_true();
        DOT_CHECK(is_class<box<uint>>::of<object>()).is_true();
        DOT_CHECK(is_class<box<uint>>::of<box_based>()).is_true();
        DOT_CHECK(is_class<box<int>>::of<box<uint>>()).is_false();
        DOT_CHECK(is_class<box<uint>>::of<box<int>>()).is_false();
        DOT_CHECK(is_class<object>::of<box<int>>()).is_false();
        DOT_CHECK(is_class<object>::of<box<uint>>()).is_false();
        DOT_CHECK(is_class<box_based>::of<box<int>>()).is_false();
        DOT_CHECK(is_class<box_based>::of<box<uint>>()).is_false();
        DOT_CHECK(is_class<object>::of<box_based>()).is_false();
        DOT_CHECK(is_class<box_based>::of<box_based>()).is_true();
        DOT_CHECK(is_class<box<int>>::of<box<int>>()).is_true();
        DOT_CHECK(is_class<box<uint>>::of<box<uint>>()).is_true();

        DOT_CHECK(is_class<box_based::cat_based>::of<object::data>()).is_true();
        DOT_CHECK(is_class<box<int>::cat>::of<object::data>()).is_true();
        DOT_CHECK(is_class<box<int>::cat>::of<box_based::cat_based>()).is_true();
        DOT_CHECK(is_class<box<uint>::cat>::of<object::data>()).is_true();
        DOT_CHECK(is_class<box<uint>::cat>::of<box_based::cat_based>()).is_true();
        DOT_CHECK(is_class<box<int>::cat>::of<box<uint>::cat>()).is_false();
        DOT_CHECK(is_class<box<uint>::cat>::of<box<int>::cat>()).is_false();
        DOT_CHECK(is_class<object::data>::of<box<int>::cat>()).is_false();
        DOT_CHECK(is_class<object::data>::of<box<uint>::cat>()).is_false();
        DOT_CHECK(is_class<box_based::cat_based>::of<box<int>::cat>()).is_false();
        DOT_CHECK(is_class<box_based::cat_based>::of<box<uint>::cat>()).is_false();
        DOT_CHECK(is_class<object::data>::of<box_based::cat_based>()).is_false();
        DOT_CHECK(is_class<box_based::cat_based>::of<box_based::cat_based>()).is_true();
        DOT_CHECK(is_class<box<int>::cat>::of<box<int>::cat>()).is_true();
        DOT_CHECK(is_class<box<uint>::cat>::of<box<uint>::cat>()).is_true();
    }

    DOT_TEST_SUITE(box_instance_hierarchy)
    {
        box_based n;
        DOT_CHECK(n).is_null();
        DOT_CHECK(n).is<box_based>();
        DOT_CHECK(n).is<object>();
        DOT_CHECK(n).is_not<box<int>>();
        DOT_CHECK(n.my_id() == box_based::id()).is_true();
        DOT_CHECK(n.my_id() == box<int>::id()).is_false();
        DOT_CHECK(n.my_id() == object::id()).is_false();

        box_based i(-1);
        DOT_CHECK(i).is_not_null();
        DOT_CHECK(box<int>(i)) == -1;
        DOT_CHECK(i).is<box_based>();
        DOT_CHECK(i).is<object>();
        DOT_CHECK(i).is_not<box<int>>();
        DOT_CHECK(i.get_data()).is<box<int>::cat>();
        DOT_CHECK(i.get_data()).is<box_based::cat_based>();
        DOT_CHECK(i.get_data()).is_not<box<uint>::cat>();
        DOT_CHECK(i.get_data()).is<object::data>();
        DOT_CHECK(i.my_id() == box_based::id()).is_true();
        DOT_CHECK(i.my_id() == box<int>::id()).is_false();
        DOT_CHECK(i.my_id() == object::id()).is_false();

        box<uint> u(0x1dea0fu);
        DOT_CHECK(u).is_not_null();
        DOT_CHECK(u) == 0x1dea0fu;
        DOT_CHECK(u).is<box<uint>>();
        DOT_CHECK(u).is_not<box<int>>();
        DOT_CHECK(u).is<box_based>();
        DOT_CHECK(u).is<object>();
        DOT_CHECK(u.get_data()).is<box<uint>::cat>();
        DOT_CHECK(u.get_data()).is<box_based::cat_based>();
        DOT_CHECK(u.get_data()).is_not<box<int>::cat>();
        DOT_CHECK(u.get_data()).is<object::data>();

        box<int> t(1234567890);
        DOT_CHECK(t).is_not_null();
        DOT_CHECK(t) == 1234567890;
        DOT_CHECK(t).is<box<int>>();
        DOT_CHECK(t).is_not<box<uint>>();
        DOT_CHECK(t).is<box_based>();
        DOT_CHECK(t).is<object>();
        DOT_CHECK(t.get_data()).is<box<int>::cat>();
        DOT_CHECK(t.get_data()).is<box_based::cat_based>();
        DOT_CHECK(t.get_data()).is_not<box<uint>::cat>();
        DOT_CHECK(t.get_data()).is<object::data>();
    }

    DOT_TEST_SUITE(box_creation)
    {
        box<long long> LL(-1234567890123456789LL);
        box<long> L(123456789L);
        box<int> i(-987654321);
        box<short> s(short(-32100));
        box<char> c('c');
        box<unsigned long long> uLL(12345678901234567890uLL);
        box<unsigned long> uL(4201234567uL);
        box<unsigned int> ui(0xbadfaced);
        box<unsigned short> us(unsigned short(0x1eaf));
        box<unsigned char> uc(unsigned char('u'));
        box<double> d(987.65432109876);
        box<float> f(54.321f);
        box<bool> b(true);

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

        box<long long> aLL = oLL;
        box<long> aL = oL;
        box<int> ai = oi;
        box<short> as = os;
        box<char> ac = oc;
        box<unsigned long long> auLL = ouLL;
        box<unsigned long> auL = ouL;
        box<unsigned int> aui = oui;
        box<unsigned short> aus = ous;
        box<unsigned char> auc = ouc;
        box<double> ad = od;
        box<float> af = of;
        box<bool> ab = ob;

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

    template<> const class_id& box<test_type>::id() noexcept
    {
        static const class_id box_test_type_id("box<test_type>");
        return box_test_type_id;
    }

    template<> const class_id& box<test_type>::cat::id() noexcept
    {
        static const class_id box_test_type_cat_id("box<test_type>::cat");
        return box_test_type_cat_id;
    }

    DOT_TEST_SUITE(box_test_type)
    {
        object x(test_type(100500uLL, 9000.1));
        DOT_ENSURE(x.get_data()).is<box<test_type>::cat>();
        DOT_ENSURE_NO_EXCEPTION(x.get_as<test_type>());
        DOT_CHECK(x.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(x.get_as<test_type>().value) == 9000.1;

        box<test_type> y = x;
        DOT_ENSURE(y).is<box<test_type>>();
        DOT_ENSURE(y.get_data()).is<box<test_type>::cat>();
        DOT_ENSURE_NO_EXCEPTION(y.get_as<test_type>());
        DOT_CHECK(y.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(y.get_as<test_type>().value) == 9000.1;

        DOT_ENSURE_NO_EXCEPTION(
            x = test_type(12345678901234567890uLL, -1234567.87654321));
        DOT_ENSURE(x.get_data()).is<box<test_type>::cat>();
        DOT_ENSURE_NO_EXCEPTION(x.get_as<test_type>());
        DOT_CHECK(x.get_as<test_type>().index) == 12345678901234567890uLL;
        DOT_CHECK(x.get_as<test_type>().value) == -1234567.87654321;

        DOT_ENSURE(y).is<box<test_type>>();
        DOT_ENSURE(y.get_data()).is<box<test_type>::cat>();
        DOT_ENSURE_NO_EXCEPTION(y.get_as<test_type>());
        DOT_CHECK(y.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(y.get_as<test_type>().value) == 9000.1;

        object z = y;
        DOT_ENSURE(z.get_data()).is<box<test_type>::cat>();
        DOT_ENSURE_NO_EXCEPTION(z.get_as<test_type>());
        DOT_CHECK(z.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(z.get_as<test_type>().value) == 9000.1;

        DOT_ENSURE_NO_EXCEPTION(z = 0);
        DOT_ENSURE(z.get_data()).is<box<int>::cat>();
        DOT_CHECK(z.get_as<int>()) == 0;

        DOT_ENSURE(y).is<box<test_type>>();
        DOT_ENSURE(y.get_data()).is<box<test_type>::cat>();
        DOT_CHECK(y.get_as<test_type>().index) == 100500uLL;
        DOT_CHECK(y.get_as<test_type>().value) == 9000.1;

        DOT_ENSURE(x.get_data()).is<box<test_type>::cat>();
        DOT_ENSURE_NO_EXCEPTION(x.get_as<test_type>());
        DOT_CHECK(x.get_as<test_type>().index) == 12345678901234567890uLL;
        DOT_CHECK(x.get_as<test_type>().value) == -1234567.87654321;
    }
}

// Здесь должен быть Unicode
