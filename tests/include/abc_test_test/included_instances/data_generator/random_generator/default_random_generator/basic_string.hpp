#pragma once
#include "abc_test/core.hpp"
#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"

// basic_string<T> tests.

namespace test
{
template <typename T, typename Rng>
inline void
    fuzzy_test_default_random_generator_basic_string(
        const std::size_t _a_max_rng_offset
        = std::numeric_limits<std::size_t>::max()
    )
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_string_tests,
        fmt::format(
            "Fuzzy testing default_random_generator_t's specialization "
            "for {0}, using RNG {1}",
            typeid(T),
            typeid(Rng)
        )
    );
    // Begin by allowing basic_strings with a maximum size to be created.
    default_random_generator_t<T> _l_rdg{
        default_random_generator<typename T::value_type>(),
        bounds_t<typename T::size_type>(
            0,
            abc::global::get_global_test_options()
                .maximum_individual_allocatable_memory<typename T::value_type>()
        )
    };
    // Make the random generator.
    rng_t _l_rng      = rng_t::make_rng<Rng>(vector<uint32_t>());
    using test_data_t = tuple<size_t, size_t>;
    // Initialize the random generator, allowing rng_seeds to be between 0 and
    // _a_max_rng_offset. With some random generators, setting this very high
    // will be computatioanlly expensive. So we allow a ceiling to be set so
    // that tests do not take too long.
    for (const auto& _l_tuple :
         generate_data_randomly<test_data_t>(
             default_random_generator<test_data_t>(
                 default_random_generator<size_t>(0, _a_max_rng_offset),
                 default_random_generator<size_t>()
             )
         )
             & iterate_over<test_data_t>({
                 {0, std::numeric_limits<size_t>::max()},
                 {0, 0                                 }
    }))
    {
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        auto& [_l_rng_seed_value, _l_size_index]{_l_tuple};
        // Seed the random generator function with the given seed value.
        _l_rng.progress(_l_rng_seed_value);
        // Generate the string.
        T _l_rv{_l_rdg.operator()(_l_rng, rng_counter_t(_l_size_index))};
        // Ensure values are not optimized away.
        do_not_optimise(_l_rv);
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_string_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_string_tests);
}

template <typename T>
inline void
    fuzzy_test_default_random_generator_basic_string()
{
    using namespace abc;
    using namespace std;
    using namespace test;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (fuzzy_test_default_random_generator_basic_string<
            T,
            utility::simple_rng_t>()));
    RUN(_l_mdg,
        (fuzzy_test_default_random_generator_basic_string<
            T,
            utility::inner_rng_mt19937_64_t>(1'000'000)));
}
} // namespace test

// TODO:
//  - Test which uses letters on the keyboard, allowing us to write the tests as
//  strings.

_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy test for random_data_generator_t for the "
                 "std::basic_string type using various RNG",
         .path = "abc_test_test::included_instances::data_generator::random_"
                 "generator::default_random_generator::basic_string",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    manual_data_generator_t _l_mdg;
    // RUN(_l_mdg, fuzzy_test_default_random_generator_basic_string<string>());
    // RUN(_l_mdg, fuzzy_test_default_random_generator_basic_string<wstring>());
    // RUN(_l_mdg,
    // fuzzy_test_default_random_generator_basic_string<u8string>());
    RUN(_l_mdg, fuzzy_test_default_random_generator_basic_string<u16string>());
    // RUN(_l_mdg,
    // fuzzy_test_default_random_generator_basic_string<u32string>());
    // RUN(_l_mdg,
    //     fuzzy_test_default_random_generator_basic_string<basic_string<int>>());
}

namespace test
{
template <typename T>
inline void
    unit_test_default_random_generator_basic_string_using_vect()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_string_tests,
        "Testing default_random_generator_t for std::basic_string produces the "
        "correct values"
    );
    default_random_generator_t<T> _l_rdg;
    rng_t _l_rng      = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
    using test_data_t = tuple<size_t, size_t, vector<typename T::value_type>>;
    for (const auto& _l_tuple : read_data_from_file<test_data_t>(
             fmt::format("string_test_data_using_vect", typeid(T))
         ))
    {
        _TVLOG_(_l_tuple);
        auto& [_l_rng_seed_value, _l_size_index, _l_result_as_signed_chars]{
            _l_tuple
        };
        // Convert signed char vector to a string
        T _l_str_result = static_cast_range<T>(_l_result_as_signed_chars);
        // Seed the random generator function with the given seed value.
        _l_rng.progress(_l_rng_seed_value);
        // Generate the string.
        T _l_rv{_l_rdg.operator()(_l_rng, utility::rng_counter_t(_l_size_index))
        };
        // Convert the gotten string to a vector of signed chars.
        auto _l_str_gotten{static_cast_range<vector<typename T::value_type>>(_l_rv)};
        _TVLOG_(_l_str_gotten);
        // Run the test.
        _l_string_tests += _BLOCK_CHECK(_EXPR(_l_str_result == _l_rv));
    }
    _END_BBA_CHECK(_l_string_tests);
}
} // namespace test

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit testing random_data_generator_t for std::basic_string "
                 "type, using user-supplied vectors of signed integers",
         .path = "abc_test_test::included_instances::data_generator::random_"
                 "generator::default_random_generator::basic_string",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    manual_data_generator_t _l_mdg;
    // RUN(_l_mdg,
    //    unit_test_default_random_generator_basic_string_using_vect<string>());
    // RUN(_l_mdg,
    // unit_test_default_random_generator_basic_string_using_vect<wstring>());
    // RUN(_l_mdg,
    // unit_test_default_random_generator_basic_string_using_vect<u8string>());
    // RUN(_l_mdg,
    //    unit_test_default_random_generator_basic_string_using_vect<u16string>()
    // );
    // RUN(_l_mdg,
    // unit_test_default_random_generator_basic_string_using_vect<u32string>()
    // );
    // RUN(_l_mdg,
    // unit_test_default_random_generator_basic_string_using_vect<
    //    basic_string<int>>());
}

namespace test
{
template <typename T>
inline void
    unit_test_default_random_generator_basic_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_string_tests,
        "Testing default_random_generator_t for std::basic_string produces the "
        "correct values"
    );
    default_random_generator_t<T> _l_rdg;
    rng_t _l_rng      = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
    using test_data_t = tuple<size_t, size_t, T>;
    for (const auto& _l_tuple : read_data_from_file<test_data_t>(
             fmt::format("string_test_data_using", typeid(T))
         ))
    {
        _TVLOG_(_l_tuple);
        auto& [_l_rng_seed_value, _l_size_index, _l_string]{_l_tuple};
        // Seed the random generator function with the given seed value.
        _l_rng.progress(_l_rng_seed_value);
        // Generate the string.
        T _l_rv{_l_rdg.operator()(_l_rng, utility::rng_counter_t(_l_size_index))
        };
        // Run the test.
        _l_string_tests += _BLOCK_CHECK(_EXPR(_l_string == _l_rv));
    }
    _END_BBA_CHECK(_l_string_tests);
}
} // namespace test

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit testing random_data_generator_t for std::basic_string "
                 "type, using user-supplied ASCII strings",
         .path = "abc_test_test::included_instances::data_generator::random_"
                 "generator::default_random_generator::basic_string",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    manual_data_generator_t _l_mdg;
    // RUN(_l_mdg, unit_test_default_random_generator_basic_string<string>());
    // RUN(_l_mdg, unit_test_default_random_generator_basic_string<wstring>());
    // RUN(_l_mdg, unit_test_default_random_generator_basic_string<u8string>());
    // RUN(_l_mdg,
    // unit_test_default_random_generator_basic_string<u16string>());
    // RUN(_l_mdg,
    // unit_test_default_random_generator_basic_string<u32string>());
    // RUN(_l_mdg,
    //     unit_test_default_random_generator_basic_string<basic_string<int>>());
}