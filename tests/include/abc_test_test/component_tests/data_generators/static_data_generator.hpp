#pragma once

#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/unicode.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic component test for static data generator",
         .path = "abc_test_test::component_tests::data_generators::static_data_"
                 "generator",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_unit_tests = _MULTI_MATCHER("Unit tests for static data generator");
    for (auto&& _l_values : read_data_from_file<vector<int>>("unit_test"))
    {
        vector<int> _l_result;
        for (auto&& _l_element : iterate_over<int>(_l_values))
        {
            _l_result.push_back(_l_element);
        }
        _l_unit_tests << _CHECK_EXPR(_l_result == _l_values);
    }
    _CHECK(_l_unit_tests);

    function<void()> _l_test_func = [&]()
    {

    };
}
