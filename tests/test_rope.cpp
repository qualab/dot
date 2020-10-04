// DOT test main

#include <dot/test.h>
#include <dot/rope.h>
#include <dot/string.h>
#include <iostream>
#include <thread>
#include <vector>

using std::string;
using std::wstring;
using std::u16string;
using std::u32string;
using std::thread;
using std::vector;

namespace dot
{
    DOT_TEST_SUITE(rope_class_hierarchy)
    {
        DOT_CHECK(is_class<rope_based>::of<object>()).is_true();
        DOT_CHECK(is_class<rope<string>>::of<object>()).is_true();
        DOT_CHECK(is_class<rope<string>>::of<rope_based>()).is_true();
        DOT_CHECK(is_class<rope<wstring>>::of<object>()).is_true();
        DOT_CHECK(is_class<rope<wstring>>::of<rope_based>()).is_true();
        DOT_CHECK(is_class<rope<string>>::of<rope<wstring>>()).is_false();
        DOT_CHECK(is_class<rope<wstring>>::of<rope<string>>()).is_false();
        DOT_CHECK(is_class<object>::of<rope<wstring>>()).is_false();
        DOT_CHECK(is_class<object>::of<rope<string>>()).is_false();
        DOT_CHECK(is_class<rope_based>::of<rope<wstring>>()).is_false();
        DOT_CHECK(is_class<rope_based>::of<rope<string>>()).is_false();
        DOT_CHECK(is_class<object>::of<rope_based>()).is_false();
        DOT_CHECK(is_class<rope_based>::of<rope_based>()).is_true();
        DOT_CHECK(is_class<rope<string>>::of<rope<string>>()).is_true();
        DOT_CHECK(is_class<rope<wstring>>::of<rope<wstring>>()).is_true();

        DOT_CHECK(is_class<rope_based::cow_based>::of<object::data>()).is_true();
        DOT_CHECK(is_class<rope<string>::cow>::of<object::data>()).is_true();
        DOT_CHECK(is_class<rope<string>::cow>::of<rope_based::cow_based>()).is_true();
        DOT_CHECK(is_class<rope<wstring>::cow>::of<object::data>()).is_true();
        DOT_CHECK(is_class<rope<wstring>::cow>::of<rope_based::cow_based>()).is_true();
        DOT_CHECK(is_class<rope<string>::cow>::of<rope<wstring>::cow>()).is_false();
        DOT_CHECK(is_class<rope<wstring>::cow>::of<rope<string>::cow>()).is_false();
        DOT_CHECK(is_class<object::data>::of<rope<wstring>::cow>()).is_false();
        DOT_CHECK(is_class<object::data>::of<rope<string>::cow>()).is_false();
        DOT_CHECK(is_class<rope_based::cow_based>::of<rope<wstring>::cow>()).is_false();
        DOT_CHECK(is_class<rope_based::cow_based>::of<rope<string>::cow>()).is_false();
        DOT_CHECK(is_class<object::data>::of<rope_based::cow_based>()).is_false();
        DOT_CHECK(is_class<rope_based::cow_based>::of<rope_based::cow_based>()).is_true();
        DOT_CHECK(is_class<rope<string>::cow>::of<rope<string>::cow>()).is_true();
        DOT_CHECK(is_class<rope<wstring>::cow>::of<rope<wstring>::cow>()).is_true();
    }

    DOT_TEST_SUITE(rope_instance_hierarchy)
    {
        rope_based n;
        DOT_CHECK(n).is_null();
        DOT_CHECK(n).is<rope_based>();
        DOT_CHECK(n).is<object>();
        DOT_CHECK(n).is_not<rope<string>>();
        DOT_CHECK(n.my_id() == rope_based::id()).is_true();
        DOT_CHECK(n.my_id() == rope<std::string>::id()).is_false();
        DOT_CHECK(n.my_id() == object::id()).is_false();

        static const std::string test_utf8 = u8"Здесь должен быть Unicode.";

        rope_based s(test_utf8);
        DOT_CHECK(s).is_not_null();
        DOT_CHECK(s).is<rope_based>();
        DOT_CHECK(s).is<object>();
        DOT_CHECK(s).is_not<rope<std::string>>();
        DOT_CHECK(s.get_data()).is<rope<std::string>::cow>();
        DOT_CHECK(s.get_data()).is<rope_based::cow_based>();
        DOT_CHECK(s.get_data()).is<object::data>();
        DOT_CHECK(s.get_data()).is_not<rope<std::wstring>::cow>();

        static const std::wstring test_wide = L"Толще Unicode - больше Unicode.";

        rope<std::wstring> w(test_wide);
        DOT_CHECK(w).is_not_null();
        DOT_CHECK(w).is<rope_based>();
        DOT_CHECK(w).is<object>();
        DOT_CHECK(w).is<rope<std::wstring>>();
        DOT_CHECK(w).is_not<rope<std::string>>();
        DOT_CHECK(w.get_data()).is<rope<std::wstring>::cow>();
        DOT_CHECK(s.get_data()).is<rope_based::cow_based>();
        DOT_CHECK(w.get_data()).is<object::data>();
        DOT_CHECK(w.get_data()).is_not<rope<std::string>::cow>();

        static const std::string test_ascii = "Nobody likes ASCII jail!";

        rope<std::string> a(test_ascii);
        DOT_CHECK(a).is_not_null();
        DOT_CHECK(a).is<rope_based>();
        DOT_CHECK(a).is<object>();
        DOT_CHECK(a).is<rope<std::string>>();
        DOT_CHECK(a).is_not<rope<std::wstring>>();
        DOT_CHECK(a.get_data()).is<rope<std::string>::cow>();
        DOT_CHECK(s.get_data()).is<rope_based::cow_based>();
        DOT_CHECK(a.get_data()).is<object::data>();
        DOT_CHECK(a.get_data()).is_not<rope<std::wstring>::cow>();
    }

    DOT_TEST_SUITE(rope_of_string)
    {
        static const char* const test_string = "copy me";
        const rope<string> str(test_string);
        DOT_ENSURE(str).is_not_null();
        DOT_ENSURE(str.unique()).is_true();
        DOT_ENSURE(str.bound()) == 1;
        DOT_CHECK(*str) == test_string;
        DOT_CHECK(str->size()) == std::strlen(test_string);

        rope<string> copy = str;
        DOT_ENSURE(str.unique()).is_false();
        DOT_ENSURE(copy.unique()).is_false();
        DOT_ENSURE(str.bound()) == 2;
        DOT_ENSURE(copy.bound()) == 2;
        DOT_CHECK(*copy) == test_string;
        DOT_CHECK_NO_EXCEPTION(copy->replace(0, 4, "check"));
        DOT_ENSURE(str.unique()).is_true();
        DOT_ENSURE(copy.unique()).is_true();
        DOT_ENSURE(str.bound()) == 1;
        DOT_ENSURE(copy.bound()) == 1;
        DOT_CHECK(*str) == test_string;
        DOT_CHECK(*copy) == string("check me");
    }

    DOT_TEST_SUITE(rope_of_thread_safety)
    {
        static const string original_text = "Copy me gently! Again and again...";
        static const string another_text = "Just a text. Nothing else.";
        static const rope<string> original = original_text;
        static const rope<string> another(another_text);
        static const uint copy_count = 1000;
        static const uint thread_count = 10;
        vector<rope<string>> copies(copy_count);
        // copy via constructor and move to avoid push_back
        // vector::push_back is not thread safe operation
        auto creation_thread_action = [&](uint id)
        {
            for (uint i = id; i < copy_count; i += thread_count)
            {
                copies[i] = rope<string>(original);
            }
        };
        vector<thread> creation_threads;
        creation_threads.reserve(thread_count);
        for (uint id = 0; id < thread_count; ++id)
        {
            creation_threads.push_back(thread(creation_thread_action, id));
        }
        uint creations_OK = 0;
        for (uint id = 0; id < thread_count; ++id)
        {
            creation_threads[id].join();
        }
        for (uint id = 0; id < thread_count; ++id)
        {
            for (uint i = id; i < copy_count; i += thread_count)
            {
                const rope<string>& copy = copies[i];
                if (*copy == original_text)
                    ++creations_OK;
            }
        }
        DOT_ENSURE(creations_OK) == copy_count;
        DOT_CHECK(original.bound()) == copy_count + 1;
        DOT_CHECK(original.unique()).is_false();
        // copy via operator of assignment
        auto assignment_thread_action = [&](uint id)
        {
            for (uint i = id; i < copy_count; i += thread_count)
            {
                copies[i] = another;
            }
        };
        vector<thread> assignment_threads;
        assignment_threads.reserve(thread_count);
        for (uint id = 0; id < thread_count; ++id)
        {
            assignment_threads.push_back(thread(assignment_thread_action, id));
        }
        uint assignments_OK = 0;
        for (uint id = 0; id < thread_count; ++id)
        {
            assignment_threads[id].join();
            for (uint i = id; i < copy_count; i += thread_count)
            {
                const rope<string>& copy = copies[i];
                if (*copy == another_text)
                    ++assignments_OK;
            }
        }
        DOT_ENSURE(assignments_OK) == copy_count;
        DOT_CHECK(original.bound()) == 1;
        DOT_CHECK(original.unique()).is_true();
        DOT_CHECK(another.bound()) == copy_count + 1;
        DOT_CHECK(another.unique()).is_false();
    }

    DOT_TEST_SUITE(object_from_string)
    {
        static const char*     const cp = u8"Unicode: Нормальная кодировка!";
        static const wchar_t*  const wp = L"Unicode: Широкие Буквы!";
        static const char16_t* const up = u"Unicode: Упитанные Буквы!";
        static const char32_t* const Up = U"Unicode: Наитолстейшие Буквы!";
        static const string     cs = cp;
        static const wstring    ws = wp;
        static const u16string  us = up;
        static const u32string  Us = Up;
        object c1(cp);
        object w1(wp);
        object u1(up);
        object U1(Up);
        DOT_CHECK(c1.get_as<string>()) == cp;
        DOT_CHECK(w1.get_as<wstring>() == wp).is_true();
        DOT_CHECK(u1.get_as<u16string>() == up).is_true();
        DOT_CHECK(U1.get_as<u32string>() == Up).is_true();
        DOT_CHECK(c1.get_as<const char*>()) == cs;
        DOT_CHECK(w1.get_as<const wchar_t*>() == ws).is_true();
        DOT_CHECK(u1.get_as<const char16_t*>() == us).is_true();
        DOT_CHECK(U1.get_as<const char32_t*>() == Us).is_true();
        DOT_CHECK(c1.get_as<const string&>()) == cp;
        DOT_CHECK(w1.get_as<const wstring&>() == wp).is_true();
        DOT_CHECK(u1.get_as<const u16string&>() == up).is_true();
        DOT_CHECK(U1.get_as<const u32string&>() == Up).is_true();
        object c2(cs);
        object w2(ws);
        object u2(us);
        object U2(Us);
        DOT_CHECK(c2.get_as<string>()) == cp;
        DOT_CHECK(w2.get_as<wstring>() == wp).is_true();
        DOT_CHECK(u2.get_as<u16string>() == up).is_true();
        DOT_CHECK(U2.get_as<u32string>() == Up).is_true();
        DOT_CHECK(c2.get_as<const char*>()) == cs;
        DOT_CHECK(w2.get_as<const wchar_t*>() == ws).is_true();
        DOT_CHECK(u2.get_as<const char16_t*>() == us).is_true();
        DOT_CHECK(U2.get_as<const char32_t*>() == Us).is_true();
        DOT_CHECK(c2.get_as<const string&>()) == cp;
        DOT_CHECK(w2.get_as<const wstring&>() == wp).is_true();
        DOT_CHECK(u2.get_as<const u16string&>() == up).is_true();
        DOT_CHECK(U2.get_as<const u32string&>() == Up).is_true();
        DOT_CHECK(c2) == c1;
        DOT_CHECK(w2) == w1;
        DOT_CHECK(u2) == u1;
        DOT_CHECK(U2) == U1;
        rope<string>    c = c1;
        rope<wstring>   w = w1;
        rope<u16string> u = u1;
        rope<u32string> U = U1;
        DOT_CHECK(c.look()) == cs;
        DOT_CHECK(w.look() == ws).is_true();
        DOT_CHECK(u.look() == us).is_true();
        DOT_CHECK(U.look() == Us).is_true();
        DOT_CHECK(c) == c1;
        DOT_CHECK(w == w1).is_true();
        DOT_CHECK(u == u1).is_true();
        DOT_CHECK(U == U1).is_true();
        DOT_CHECK(c) == c2;
        DOT_CHECK(w == w2).is_true();
        DOT_CHECK(u == u2).is_true();
        DOT_CHECK(U == U2).is_true();
        c = c2;
        w = w2;
        u = u2;
        U = U2;
        DOT_CHECK(c.look()) == cp;
        DOT_CHECK(w.look() == wp).is_true();
        DOT_CHECK(u.look() == up).is_true();
        DOT_CHECK(U.look() == Up).is_true();
        DOT_CHECK(c) == c1;
        DOT_CHECK(w) == w1;
        DOT_CHECK(u) == u1;
        DOT_CHECK(U) == U1;
        DOT_CHECK(c) == c2;
        DOT_CHECK(w) == w2;
        DOT_CHECK(u) == u2;
        DOT_CHECK(U) == U2;
    }
}

// Здесь должен быть Unicode
