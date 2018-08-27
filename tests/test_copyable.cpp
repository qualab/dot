// DOT test main

#include <dot/test.h>
#include <dot/copyable.h>
#include <iostream>
#include <string>

using std::string;

namespace dot
{
    DOT_TEST_SUITE(copyable_string)
    {
        static const char* const test_string = "copy me";
        const copyable<string> str(test_string);
        DOT_TEST_ENSURE(str).is_not_null();
        DOT_TEST_ENSURE(str.unique_ref()).is_true();
        DOT_TEST_ENSURE(str.ref_counter()) == 1;
        DOT_TEST_CHECK(*str) == test_string;
        DOT_TEST_CHECK(str->size()) == std::strlen(test_string);
        copyable<string> copy = str;
        DOT_TEST_ENSURE(str.unique_ref()).is_false();
        DOT_TEST_ENSURE(copy.unique_ref()).is_false();
        DOT_TEST_ENSURE(str.ref_counter()) == 2;
        DOT_TEST_ENSURE(copy.ref_counter()) == 2;
        DOT_TEST_CHECK(*copy) == test_string;
        DOT_TEST_CHECK_NO_EXCEPTION(copy->replace(0, 4, "check"));
        DOT_TEST_ENSURE(str.unique_ref()).is_true();
        DOT_TEST_ENSURE(copy.unique_ref()).is_true();
        DOT_TEST_ENSURE(str.ref_counter()) == 1;
        DOT_TEST_ENSURE(copy.ref_counter()) == 1;
        DOT_TEST_CHECK(*copy) == "check me";
        DOT_TEST_CHECK(*str) == "copy me";
    }
}

// Unicode signature: Владимир Керимов
