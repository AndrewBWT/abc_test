#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <stdfloat>

namespace test
{
template <typename T>
requires std::floating_point<T>
inline void
    run_fp_min_addition_tests()
{
    using namespace abc;
    using namespace std;
    // As equals and less than are implemented using the built in comparison
    // function, we won't test those.
    // All we need to test is increment, decrement and difference.

    // We'll begin small at 0 and the smallest positive number representable by
    // T.
    T      _l_min_val{std::nextafter(T{0}, std::numeric_limits<T>::max())};
    T      _l_value{0};
    size_t _l_size_t{1};
    data_gen::default_enumeration_t<T> _l_enumerator(_l_min_val);
    for (size_t _l_idx{0}; _l_idx < 100'000; ++_l_idx)
    {
        size_t _l_tmp_size_t{_l_idx + 1};
        T      _l_value2{0};
        _l_size_t = 1;
       
            _l_enumerator.increment(_l_value, _l_size_t, optional<T>{})
        ;
        
            _l_enumerator.increment(_l_value2, _l_tmp_size_t, optional<T>{})
        ;
        _REQUIRE(_EXPR(_l_value == _l_value2));
    }
}

template <typename T>
inline void
    run_vector_difference_tests()
{
    using namespace abc;
    using namespace std;
    using unit_test_t = tuple<vector<T>, T, T, T>;
    for (auto& _l_element : read_data_from_file<unit_test_t>(
             gdf(string("vector_difference_").append(typeid(T).name()))
         ))
    {
        auto& [_l_end_vector, _l_t_inner_increment, _l_t_inner_min_val, _l_t_inner_max_val]
            = _l_element;
        _TVLOG(_l_element);
        data_gen::enumeration_t<vector<T>> _l_enumeration
            = default_enumeration<vector<T>>(from_m_to_n(
                _l_t_inner_min_val,
                _l_t_inner_max_val,
                default_enumeration<T>(_l_t_inner_increment)
            ));
        data_gen::enumeration_diff_t _l_diff{
            _l_enumeration->difference({}, _l_end_vector)
        };
        data_gen::enumerate_index_t _l_idx{0};
        for (auto& _l_vector : enumerate_data<vector<T>>(
                 from_min_to_val(_l_end_vector, _l_enumeration)
             ))
        {
            ++_l_idx;
            std::cout << _l_vector.size() << std::endl;
        }
        if (_l_diff.second != 0)
        {
            ++_l_diff.first;
        }
        _REQUIRE_EXPR((_l_idx - 1) == _l_diff.first);
    }
}

template <typename T>
inline void
    run_vector_tests()
{
    run_vector_difference_tests<T>();
}
} // namespace test

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing default_enumerator_t for floating point types",
         .description
         = "Checks default_enumerator_t functions for floating point numbers",
         .path
         = "abc_test_test::included_instances::data_generator::enumeration::default_enumeration",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    manual_data_generator_t _l_mgd;
    // RUN(_l_mgd, (run_fp_min_addition_tests<float>()));
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing default_enumerator_t for std::vector",
         .description
         = "Checks default_enumerator_t functions for std::vector objects",
         .path
         = "abc_test_test::included_instances::data_generator::enumeration::default_enumeration",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    manual_data_generator_t _l_mgd;
    RUN(_l_mgd, (run_vector_tests<float>()));
}