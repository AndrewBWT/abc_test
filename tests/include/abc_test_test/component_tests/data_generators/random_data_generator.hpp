#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic random generator testing values for int",
         .path = "abc_test_test::component_tests::data_generators::random_data"
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
    using namespace _ABC_NS_DS;
    vector<pair<int, int>> _l_bounds = {
        {1,      1     },
        {1,      20    },
        {-5,     17    },
        {100,    600   },
        {-1'000, 12'345}
    };
    auto unit_tests = _MULTI_MATCHER("Checking randomly generated int values");
    for (auto& [_l_min, _l_max] : _l_bounds)
    {
        vector<int> _l_results;
        for (auto&& _l_element : generate_data_randomly<int>(
                 default_random_generator<int>(_l_min, _l_max)
             ))
        {
            _l_results.push_back(_l_element);
        }
        for (auto& _l_result : _l_results)
        {
            unit_tests << _CHECK(
                _EXPR(_l_result >= _l_min) && _EXPR(_l_result <= _l_max)
            );
        }
    }
    _CHECK(unit_tests);
}
