// DOT test main

#include <dot/test.h>
#include <dot/copyable.h>
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
    DOT_TEST_SUITE(copyable_string)
    {
        static const char* const test_string = "copy me";
        const copyable<string> str(test_string);
        DOT_ENSURE(str).is_not_null();
        DOT_ENSURE(str.unique_ref()).is_true();
        DOT_ENSURE(str.ref_count()) == 1;
        DOT_CHECK(*str) == test_string;
        DOT_CHECK(str->size()) == std::strlen(test_string);

        copyable<string> copy = str;
        DOT_ENSURE(str.unique_ref()).is_false();
        DOT_ENSURE(copy.unique_ref()).is_false();
        DOT_ENSURE(str.ref_count()) == 2;
        DOT_ENSURE(copy.ref_count()) == 2;
        DOT_CHECK(*copy) == test_string;
        DOT_CHECK_NO_EXCEPTION(copy->replace(0, 4, "check"));
        DOT_ENSURE(str.unique_ref()).is_true();
        DOT_ENSURE(copy.unique_ref()).is_true();
        DOT_ENSURE(str.ref_count()) == 1;
        DOT_ENSURE(copy.ref_count()) == 1;
        DOT_CHECK(*str) == test_string;
        DOT_CHECK(*copy) == string("check me");
    }

    DOT_TEST_SUITE(copyable_thread_safety)
    {
        static const string original_text = "Copy me gently! Again and again...";
        static const string another_text = "Just a text. Nothing else.";
        static const copyable<string> original = original_text;
        static const copyable<string> another(another_text);
        static const uint copy_count = 1000;
        static const uint thread_count = 10;
        vector<copyable<string>> copies(copy_count);
        // copy via constructor and move to avoid push_back
        // vector::push_back is not thread safe operation
        auto creation_thread_action = [&](uint id)
        {
            for (uint i = id; i < copy_count; i += thread_count)
            {
                copies[i] = copyable<string>(original);
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
                const copyable<string>& copy = copies[i];
                if (*copy == original_text)
                    ++creations_OK;
            }
        }
        DOT_ENSURE(creations_OK) == copy_count;
        DOT_CHECK(original.ref_count()) == copy_count + 1;
        DOT_CHECK(original.unique_ref()).is_false();
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
                const copyable<string>& copy = copies[i];
                if (*copy == another_text)
                    ++assignments_OK;
            }
        }
        DOT_ENSURE(assignments_OK) == copy_count;
        DOT_CHECK(original.ref_count()) == 1;
        DOT_CHECK(original.unique_ref()).is_true();
        DOT_CHECK(another.ref_count()) == copy_count + 1;
        DOT_CHECK(another.unique_ref()).is_false();
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
        object c1 = cp;
        object w1 = wp;
        object u1 = up;
        object U1 = Up;
        DOT_CHECK(c1.get_as<string>()) == cp;
        DOT_CHECK(w1.get_as<wstring>()) == wp;
        DOT_CHECK(u1.get_as<u16string>()) == up;
        DOT_CHECK(U1.get_as<u32string>()) == Up;
        DOT_CHECK(c1.get_as<const char*>()) == cs;
        DOT_CHECK(w1.get_as<const wchar_t*>()) == ws;
        DOT_CHECK(u1.get_as<const char16_t*>()) == us;
        DOT_CHECK(U1.get_as<const char32_t*>()) == Us;
        DOT_CHECK(c1.get_as<const string&>()) == cp;
        DOT_CHECK(w1.get_as<const wstring&>()) == wp;
        DOT_CHECK(u1.get_as<const u16string&>()) == up;
        DOT_CHECK(U1.get_as<const u32string&>()) == Up;
        object c2 = cs;
        object w2 = ws;
        object u2 = us;
        object U2 = Us;
        DOT_CHECK(c2.get_as<string>()) == cp;
        DOT_CHECK(w2.get_as<wstring>()) == wp;
        DOT_CHECK(u2.get_as<u16string>()) == up;
        DOT_CHECK(U2.get_as<u32string>()) == Up;
        DOT_CHECK(c2.get_as<const char*>()) == cs;
        DOT_CHECK(w2.get_as<const wchar_t*>()) == ws;
        DOT_CHECK(u2.get_as<const char16_t*>()) == us;
        DOT_CHECK(U2.get_as<const char32_t*>()) == Us;
        DOT_CHECK(c2.get_as<const string&>()) == cp;
        DOT_CHECK(w2.get_as<const wstring&>()) == wp;
        DOT_CHECK(u2.get_as<const u16string&>()) == up;
        DOT_CHECK(U2.get_as<const u32string&>()) == Up;
        copyable<string>    c = c1;
        copyable<wstring>   w = w1;
        copyable<u16string> u = u1;
        copyable<u32string> U = U1;
        DOT_CHECK(c.get()) == cs;
        DOT_CHECK(w.get()) == ws;
        DOT_CHECK(u.get()) == us;
        DOT_CHECK(U.get()) == Us;
        c = c2;
        w = w2;
        u = u2;
        U = U2;
        DOT_CHECK(c.get()) == cp;
        DOT_CHECK(w.get()) == wp;
        DOT_CHECK(u.get()) == up;
        DOT_CHECK(U.get()) == Up;
    }
}

// Unicode signature: Владимир Керимов
