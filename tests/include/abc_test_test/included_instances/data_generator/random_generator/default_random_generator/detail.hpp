#pragma once
#include "abc_test/core.hpp"
#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"

// Detail tests.

namespace test::detail
{

template <typename T, typename Rng>
inline void
    test_generate_rng_value_between_bounds()
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using test_data_t = tuple<size_t, T, T>;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzz_tests,
        fmt::format(
            "Fuzzy testing generate_rng_value_between_bounds "
            "for {0}, using RNG {1}",
            typeid(T),
            typeid(Rng)
        )
    );
    for (auto& _l_tuple :
         generate_data_randomly<test_data_t>()
             & enumerate_data<test_data_t>(
                 all_values<test_data_t>(default_enumeration<test_data_t>(
                     enumerate_list<size_t>(
                         {size_t{0}, numeric_limits<size_t>::max()}
                     ),
                     enumerate_list<T>({T{0}, numeric_limits<T>::max()}),
                     enumerate_list<T>({T{0}, numeric_limits<T>::max()})
                 ))
             ))
    {
        const auto& [_l_rng_counter, _l_lower_bound, _l_higher_bound]{_l_tuple};
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        rng_t       _l_rng = rng_t::make_rng<Rng>(vector<uint32_t>());
        bounds_t<T> _l_bounds(_l_lower_bound, _l_higher_bound);
        const T     _l_result
            = abc::data_gen::detail::generate_rng_value_between_bounds(
                _l_bounds, _l_rng_counter, _l_rng
            );
        do_not_optimise(_l_result);
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzz_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzz_tests);
}

template <typename T>
inline void
    test_generate_rng_value_between_bounds()
{
    using namespace abc;
    using namespace std;
    using namespace test::str;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (test_generate_rng_value_between_bounds<T, utility::simple_rng_t>()));
    RUN(_l_mdg,
        (test_generate_rng_value_between_bounds<
            T,
            utility::inner_rng_mt19937_64_t>()));
}
} // namespace test::detail

/*_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy testing detail::generate_rng_value_between_bounds for "
                 "various types and values",
         .path
         = "abc_test_test::included_instances::data_generator::random::detail",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test::detail;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, test_generate_rng_value_between_bounds<bool>());
    RUN(_l_mdg, test_generate_rng_value_between_bounds<unsigned char>());
    RUN(_l_mdg, test_generate_rng_value_between_bounds<unsigned short>());
    RUN(_l_mdg, test_generate_rng_value_between_bounds<unsigned int>());
    RUN(_l_mdg, test_generate_rng_value_between_bounds<unsigned long>());
    RUN(_l_mdg, test_generate_rng_value_between_bounds<unsigned long long>());
}*/

namespace test::detail
{

template <typename T>
inline void
    unit_test_rng_value_between_bounds()
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using test_data_t = tuple<size_t, T, T, T>;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        fmt::format(
            "Unit test generate_rng_value_between_bounds "
            "for {0}, using RNG simple_rng_t",
            typeid(T)
        )
    );
    for (auto& _l_tuple : read_data_from_file<test_data_t>(
             fmt::format("generate_rng_value_between_bounds_{0}", typeid(T))
         ))
    {
        const auto& [_l_rng_counter, _l_lower_bound, _l_higher_bound, _l_expected_result]{
            _l_tuple
        };
        _TVLOG_(_l_rng_counter);
        rng_t       _l_rng = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
        bounds_t<T> _l_bounds(_l_lower_bound, _l_higher_bound);
        const T     _l_result
            = abc::data_gen::detail::generate_rng_value_between_bounds(
                _l_bounds, _l_rng_counter, _l_rng
            );
        _l_unit_tests += _BLOCK_CHECK(_EXPR(_l_expected_result == _l_result));
    }
    _END_BBA_CHECK(_l_unit_tests);
}

} // namespace test::detail

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests for detail::generate_rng_value_between_bounds",
         .path
         = "abc_test_test::included_instances::data_generator::random::detail",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test::detail;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, unit_test_rng_value_between_bounds<bool>());
    RUN(_l_mdg, unit_test_rng_value_between_bounds<unsigned char>());
    RUN(_l_mdg, unit_test_rng_value_between_bounds<unsigned short>());
    RUN(_l_mdg, unit_test_rng_value_between_bounds<unsigned int>());
    RUN(_l_mdg, unit_test_rng_value_between_bounds<unsigned long>());
    RUN(_l_mdg, unit_test_rng_value_between_bounds<unsigned long long>());
}