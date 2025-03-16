#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp"
#include "abc_test/utility/bounds.hpp"

template <typename T>
inline void
    test_bounds(
        abc::multi_element_test_block_t& _a_bba,
        const abc::utility::bounds_t<T>& _a_bounds,
        const T&                         _a_upper,
        const T&                         _a_lower,
        const T&                         _a_difference
    ) noexcept
{
    _a_bba += _BLOCK_CHECK(_EXPR(_a_lower == _a_bounds.lower()));
    _a_bba += _BLOCK_CHECK(_EXPR(_a_upper == _a_bounds.higher()));
    _a_bba += _BLOCK_CHECK(_EXPR(_a_difference == _a_bounds.difference()));
}

template <typename T>
inline void
    run_bounds_tests()
{
    using namespace abc;
    using namespace abc::utility;
    using namespace std;
    using namespace abc::data_gen;
    using test_data_t = std::tuple<T, T>;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_bounds_tests,
        fmt::format("Running bounds_t tests for type {0}", typeid(T))
    );
    _ABC_NS_DG::data_generator_collection_t<test_data_t> _l_data_gen;
    if constexpr (sizeof(T) <= 1)
    {
        _l_data_gen = enumerate_data<test_data_t>();
    }
    else
    {
        _l_data_gen = generate_data_randomly<test_data_t>();
    }
    for (const auto& [_l_first_val, _l_second_val] : _l_data_gen)
    {
        _TVLOG(_l_first_val);
        _TVLOG(_l_second_val);
        bounds_t<T> _l_bounds(_l_first_val, _l_second_val);
        const bool  _l_first_val_smallest{_l_first_val < _l_second_val};
        const T& _l_lower{_l_first_val_smallest ? _l_first_val : _l_second_val};
        const T& _l_upper{_l_first_val_smallest ? _l_second_val : _l_first_val};
        const T  _l_difference{static_cast<T>(
            _l_first_val_smallest ? (_l_second_val - _l_first_val)
                                  : (_l_first_val - _l_second_val)
        )};
        test_bounds(
            _l_bounds_tests, _l_bounds, _l_upper, _l_lower, _l_difference
        );
        _l_bounds = bounds_t<T>(_l_second_val, _l_first_val);
        test_bounds(
            _l_bounds_tests, _l_bounds, _l_upper, _l_lower, _l_difference
        );
    }
    _END_BBA_CHECK(_l_bounds_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing bounds_t constructors using hand-picked values",
         .path = "abc_test_test::utility::bounds",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_bounds_tests, "Testing hand-written values for bounds_t"
    );
    using test_data_t = tuple<size_t, size_t, size_t>;
    for (const auto& [_l_first_val, _l_second_val, _l_difference] :
         read_data_from_file<test_data_t>("bounds_t_handwritten_data"))
    {
        bounds_t      _l_bounds(_l_first_val, _l_second_val);
        const size_t& _l_lower{
            _l_first_val < _l_second_val ? _l_first_val : _l_second_val
        };
        const size_t& _l_upper{
            _l_first_val < _l_second_val ? _l_first_val : _l_second_val
        };
        test_bounds(
            _l_bounds_tests, _l_bounds, _l_lower, _l_upper, _l_difference
        );
        _l_bounds = bounds_t(_l_second_val, _l_first_val);
        test_bounds(
            _l_bounds_tests, _l_bounds, _l_lower, _l_upper, _l_difference
        );
    }
    _END_BBA_CHECK(_l_bounds_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing bounds_t constructors using randomly generated "
                 "values for different types",
         .path = "abc_test_test::utility::bounds",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, run_bounds_tests<std::size_t>());
    RUN(_l_mdg, run_bounds_tests<std::uint8_t>());
}