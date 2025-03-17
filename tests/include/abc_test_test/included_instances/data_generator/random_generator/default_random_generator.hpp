#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"

// TODO:
//  - Test which uses letters on the keyboard, allowing us to write the tests as
//  strings.

_TEST_CASE(
    abc::test_case_t(
        {.name = "Coverage test for random_data_generator_t, for the "
                 "std::string type",
         .path = "abc_test_test::included_instances::data_generator::random",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_string_tests,
        "Testing randomly generated values for default_random_generator_t for "
        "std::string, to ensure no crashes"
    );
    default_random_generator_t<string> _l_rdg;
    rng _l_rng        = rng::make_rng<simple_rng_t>(vector<uint32_t>());
    using test_data_t = tuple<size_t, size_t>;
    for (const auto& _l_tuple : generate_data_randomly<test_data_t>())
    {
        auto& [_l_rng_seed_value, _l_size_index]{_l_tuple};
        // Seed the random generator function with the given seed value.
        _l_rng.progress(_l_rng_seed_value);
        // Generate the string.
        string _l_rv{_l_rdg.operator()(_l_rng, rng_counter_t(_l_size_index))};
        // Ensure values are not optimized away.
        do_not_optimise(_l_rv);
    }
    _END_BBA_CHECK(_l_string_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing random_data_generator_t for std::string type, using "
                 "user-supplied vectors of signed integers",
         .path = "abc_test_test::included_instances::data_generator::random",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_string_tests,
        "Testing default_random_generator_t for std::string produces the "
        "correct values"
    );
    default_random_generator_t<string> _l_rdg;
    rng _l_rng        = rng::make_rng<simple_rng_t>(vector<uint32_t>());
    using test_data_t = tuple<size_t, size_t, vector<signed char>>;
    for (const auto& _l_tuple :
         read_data_from_file<test_data_t>("string_test_data"))
    {
        auto& [_l_rng_seed_value, _l_size_index, _l_result_as_signed_chars]{
            _l_tuple
        };
        // Convert signed char vector to a string
        string _l_str_result = convert_type<string>(_l_result_as_signed_chars);
        // Seed the random generator function with the given seed value.
        _l_rng.progress(_l_rng_seed_value);
        // Generate the string.
        string _l_rv{_l_rdg.operator()(_l_rng, utility::rng_counter_t(_l_size_index))};
        // Convert the gotten string to a vector of signed chars.
        auto _l_str_gotten{convert_type<vector<signed char>>(_l_rv)};
        // Run the test.
        _l_string_tests += _BLOCK_CHECK(annotate(
            fmt::format(
                "Checking that entity {0} produces correct results, with "
                "string \"{1}\". Result (as signed chars) = {2}",
                _l_tuple,
                _l_str_result,
                _l_str_gotten
            ),
            _EXPR(_l_str_result == _l_rv)
        ));
    }
    _END_BBA_CHECK(_l_string_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing random_data_generator_t for std::string type, using "
                 "user-supplied strings",
         .path = "abc_test_test::included_instances::data_generator::random",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_string_tests,
        "Testing default_random_generator_t for std::string produces the "
        "correct values"
    );
    default_random_generator_t<string> _l_rdg(
        default_random_generator<char>(' ', '~')
    );
    rng _l_rng        = rng::make_rng<simple_rng_t>(vector<uint32_t>());
    using test_data_t = tuple<size_t, size_t, string>;
    for (const auto& _l_tuple :
         read_data_from_file<test_data_t>("string_test_data"))
    {
        auto& [_l_rng_seed_value, _l_size_index, _l_result_as_str]{_l_tuple};
        // Seed the random generator function with the given seed value.
        _l_rng.progress(_l_rng_seed_value);
        // Generate the string.
        string _l_rv{_l_rdg.operator()(_l_rng, rng_counter_t(_l_size_index))};
        // Run the test.
        _l_string_tests += _BLOCK_CHECK(annotate(
            fmt::format(
                "Checking that entity {0} produces correct results", _l_tuple
            ),
            _EXPR(_l_result_as_str == _l_rv)
        ));
    }
    _END_BBA_CHECK(_l_string_tests);
}