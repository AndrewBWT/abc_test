#pragma once
#include "abc_test/core.hpp"
#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"
#include "abc_test_test/testing_utilities/functions.hpp"

// basic_string<T> tests.

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
    using namespace utility;
    using namespace abc::data_gen;
    using data_types_t = abc::utility::type_list<
        string,
        u8string,
        u16string,
        u32string,
        wstring
    //    basic_string<int>
    >;
    using data_types_2_t = abc::utility::
        type_list<utility::simple_rng_t, utility::inner_rng_mt19937_64_t>;
    auto _l_test_func = [&]<typename T, typename Rng>(
                            const size_t _a_max_rng_offset
                            = std::numeric_limits<std::size_t>::max()
                        )
    {
        auto _l_string_tests = _MULTI_MATCHER(fmt::format(
            "Fuzzy testing default_random_generator_t's specialization "
            "for {0}, using RNG {1}",
            typeid(T),
            typeid(Rng)
        ));
        // Begin by allowing basic_strings with a maximum size to be created.
        default_random_generator_t<T> _l_rdg{
            default_random_generator<typename T::value_type>(),
            bounds_t<typename T::size_type>(
                0,
                abc::global::get_global_test_options()
                    .maximum_individual_allocatable_memory<
                        typename T::value_type>()
            )
        };
        // Make the random generator.
        rng_t _l_rng      = rng_t::make_rng<Rng>(vector<uint32_t>());
        using test_data_t = tuple<size_t, size_t>;
        // Initialize the random generator, allowing rng_seeds to be between 0
        // and _a_max_rng_offset. With some random generators, setting this very
        // high will be computatioanlly expensive. So we allow a ceiling to be
        // set so that tests do not take too long.
        for (const auto& [_l_rng_seed_value, _l_size_index] :
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
            // Seed the random generator function with the given seed value.
            _l_rng.progress(_l_rng_seed_value);
            // Generate the string.
            T _l_rv{_l_rdg.operator()(_l_rng, rng_counter_t(_l_size_index))};
            // Ensure values are not optimized away.
            do_not_optimise(_l_rv);
            // End the checks for an exception being thrown.
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_string_tests << _CHECK(_l_matcher);
        }
        _CHECK(_l_string_tests);
    };

    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T1>()
        {
            auto _l_second_level = [&]()
            {
                manual_data_generator_t _l_mdg2;
                RUN(_l_mdg2,
                    (_l_test_func.operator()<T1, utility::simple_rng_t>()));
                RUN(_l_mdg2,
                    (_l_test_func.operator(
                    )<T1, utility::inner_rng_mt19937_64_t>(1'000'000)));
            };
            RUN(_l_mdg, (_l_second_level()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit testing random_data_generator_t",
         .path = "abc_test_test::random_"
                 "generator::default_random_generator::basic_string",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    using namespace abc::data_gen;
    using namespace abc::utility;
    using namespace abc_test::utility;
    using data_types_t = abc::utility::type_list<
        string,
        u8string,
        u16string,
        u32string,
        wstring
     //   basic_string<int>
     >;
    manual_data_generator_t _l_mdg;
    auto                    _l_test_func = [&]<typename T>()
    {
        auto _l_string_tests
            = _MULTI_MATCHER("Testing default_random_generator_t for "
                             "std::basic_string produces the "
                             "correct values");
        default_random_generator_t<T> _l_rdg;
        rng_t _l_rng = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
        using test_data_t
            = tuple<size_t, size_t, vector<typename T::value_type>>;
        for (const auto& [_l_rng_seed_value, _l_size_index, _l_result_as_signed_chars] :
             read_data_from_file<test_data_t>(
                 fmt::format("string_test_data_using_vect", typeid(T))
             ))
        {
            // Convert signed char vector to a string
            T _l_str_result = static_cast_range<T>(_l_result_as_signed_chars);
            // Seed the random generator function with the given seed value.
            _l_rng.progress(_l_rng_seed_value);
            // Generate the string.
            T _l_rv{
                _l_rdg.operator()(_l_rng, utility::rng_counter_t(_l_size_index))
            };
            // Convert the gotten string to a vector of signed chars.
            auto _l_str_gotten{
                static_cast_range<vector<typename T::value_type>>(_l_rv)
            };
            _TVLOG_(_l_str_gotten);
            // Run the test.
            _l_string_tests << _CHECK(_EXPR(_l_str_result == _l_rv));
        }
        _CHECK(_l_string_tests);
    };
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit testing random_dat",
         .path = "abc_test_string",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    using namespace utility;
    using namespace abc::data_gen;
    using data_types_t = abc::utility::type_list<
        string,
        u8string,
        u16string,
        u32string,
        wstring
        //basic_string<int>
        >;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_string_tests
            = _MULTI_MATCHER("Testing default_random_generator_t for "
                             "std::basic_string produces the "
                             "correct values");
        default_random_generator_t<T> _l_rdg;
        rng_t _l_rng      = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
        using test_data_t = tuple<size_t, size_t, T>;
        for (const auto& [_l_rng_seed_value, _l_size_index, _l_string] :
             read_data_from_file<test_data_t>(
                 fmt::format("string_test_data_using", typeid(T))
             ))
        {
            // Seed the random generator function with the given seed value.
            _l_rng.progress(_l_rng_seed_value);
            // Generate the string.
            T _l_rv{
                _l_rdg.operator()(_l_rng, utility::rng_counter_t(_l_size_index))
            };
            // Run the test.
            _l_string_tests << _CHECK(_EXPR(_l_string == _l_rv));
        }
        _CHECK(_l_string_tests);
    };
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}