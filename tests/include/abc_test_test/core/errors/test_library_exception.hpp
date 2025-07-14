#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <type_traits>

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing traits of of test_library_exception_t",
         .path = "abc_test_test::core::errors::test_library_exception",
         .threads_required = 1}
    )
)
{
    using namespace _ABC_NS_ERRORS;
    using namespace abc;
    _CHECK(annotate(
        u8"Checks test_library_exception_t is derived from std::runtime_error",
        _EXPR(
            ( std::is_base_of_v<std::runtime_error, test_library_exception_t> )
            == true
        )
    ));
    _CHECK(annotate(
        u8"Checks test_library_exception_t is not default constructable",
        _EXPR(not std::is_default_constructible_v<test_library_exception_t>)
            == true
    ));
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing constructors and getters of test_library_exception_t",
         .path = "abc_test::core::errors",
         .threads_required = 1}
    )
)
{
    using namespace _ABC_NS_ERRORS;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace abc;
    using namespace std;
    auto _l_test_lib = _MULTI_MATCHER("Testing properties of test_library_exception_t");
    for (const string& _l_str : generate_data_randomly<string>())
    {
        stacktrace               _l_st = stacktrace::current();
        test_library_exception_t _l_tle
            = test_library_exception_t(cast_string_to_u8string(_l_str), _l_st);
        _l_test_lib << _CHECK(annotate(
            u8"Checking test_library_exception_t what() function",
            _EXPR(string_view(_l_tle.what()) == string_view(_l_str))
        ));
        _l_test_lib << _CHECK(annotate(
            u8"Checking test_library_exception_t source_location() function",
            stacktraces_equal(_l_tle.stacktrace(), _l_st)
        ));
    }
    _CHECK(_l_test_lib);
}