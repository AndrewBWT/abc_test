#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Testing set_complete_global_seed",
         .path             = "abc_test_test::utility::rng::rng_utilities",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_property_tests, fmt::format("Property testing set_complete_global_seed")
    );
    using test_1 = tuple<complete_global_seed_t>;
    for (const auto& _l_data : generate_data_randomly<test_1>())
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed]{_l_data};
        auto _l_result{
            abc::utility::set_complete_global_seed(make_optional(_l_seed))
        };
        _l_property_tests += _BLOCK_CHECK(_EXPR(_l_seed == _l_result));
    }
    _END_BBA_CHECK(_l_property_tests);

    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, fmt::format("Fuzzy testing set_complete_global_seed")
    );
    using test_2 = tuple<global_seed_t>;
    for (const auto& _l_data : generate_data_randomly<test_2>())
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed]{_l_data};
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(abc::utility::set_complete_global_seed(_l_seed));
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}
