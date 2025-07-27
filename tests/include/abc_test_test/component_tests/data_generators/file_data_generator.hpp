#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic component tests for file data generator",
         .path = "abc_test_test::component_tests::data_generators::file_data_"
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

    auto _l_unit_tests
        = _MULTI_MATCHER("Testing file creation using file_data_generator");

    function<void()> _l_test_func_1 = [&]()
    {
        for (auto&& _l_element : read_data_from_file<int>("test_file"))
        {
        }
    };
    _CHECK(_l_unit_tests);
}
