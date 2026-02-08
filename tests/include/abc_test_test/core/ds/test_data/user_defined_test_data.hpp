#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/std_specializations.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Tests for equals comparison operator for user_defined_test_data_t",
         .description
         = "Checks that each of the functions for user_defined_test_data_t are "
           "able to process randomised data",
         .path = "abc_test_test::core::ds::test_data::user_defined_test_data",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::ds;
    auto _l_fuzzy_tests
        = _MULTI_MATCHER(fmt::format("Fuzzy tests for equals comparison "
                                     "operator for user_defined_test_data_t"));
    using test_data_t = tuple<string, optional<string>, string, size_t>;
    for (auto& [_l_test_name, _l_test_description, _l_path, _l_n_threads] :
         generate_data_randomly<test_data_t>())
    {
        user_defined_test_data_t _l_udtd{
            _l_test_name, _l_test_description, _l_path, _l_n_threads
        };
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        abc::do_not_optimise(_l_udtd == _l_udtd);
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests << _CHECK(_l_matcher);
    }
    _CHECK(_l_fuzzy_tests);
}