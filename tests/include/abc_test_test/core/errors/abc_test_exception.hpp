#pragma once

#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <type_traits>

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Testing traits of of abc_test_exception_t",
         .path             = "abc_test_test::core::errors::abc_test_exception",
         .threads_required = 1}
    )
)
{
    using namespace _ABC_NS_ERRORS;
    using namespace abc;
    _CHECK(annotate(
        u8"Checks abc_test_library_exception_t is derived from std::exception",
        _EXPR(
            ( std::is_base_of_v<std::exception, abc_test_exception_t> ) == true
        )
    ));
    _CHECK(annotate(
        u8"Checks abc_test_library_exception_t is not default constructable",
        _EXPR(not std::is_default_constructible_v<abc_test_exception_t>) == true
    ));
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing constructors and getters of abc_test_exception_t",
         .path = "abc_test::core::errors",
         .threads_required = 1}
    )
)
{
    using namespace _ABC_NS_ERRORS;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace abc;
    using namespace std;
    auto _l_test_lib
        = _MULTI_MATCHER("Testing properties of abc_test_exception_t");
    for (const auto& [_l_str, _l_terminated] :
         generate_data_randomly<pair<vector<u8string>, bool>>())
    {
        u8string _l_combined = [&]()
        {
            u8string _l_rv;
            for (size_t _l_idx{0}; _l_idx < _l_str.size(); ++_l_idx)
            {
                _l_rv.append(_l_str[_l_idx]);
                if (_l_idx + 1 < _l_str.size())
                {
                    _l_rv.append(u8"\n");
                }
            }
            return _l_rv;
        }();
        stacktrace           _l_st = stacktrace::current();
        abc_test_exception_t _l_tle
            = abc_test_exception_t(_l_str, _l_terminated, _l_st);
        abc_test_error_t _l_te{_l_str, _l_terminated, _l_st};
        string           _l_what_str{_l_tle.what()};
        u8string         _l_what_in_u8string{
            u8string(_l_what_str.begin(), _l_what_str.end())
        };
        _l_test_lib << _CHECK(annotate(
            u8"Checking abc_test_exception_t what() function",
            _EXPR(_l_what_in_u8string == _l_combined)
        ));
        auto ki1 = std::make_tuple(
            _l_tle.internal_error().errors(),
            _l_tle.internal_error().terminates_test_framework()
        );
        auto ki2 = std::make_tuple(
            _l_te.errors(), _l_te.terminates_test_framework()
        );
        _l_test_lib << _CHECK(_EXPR(ki1 == ki2));
    }
    _CHECK(_l_test_lib);
}