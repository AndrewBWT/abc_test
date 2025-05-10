#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp"
#include "abc_test/utility/bounds.hpp"

namespace
{

using container_type_list_t = abc::utility::type_list<
#if _TESTING_BUILD
    bool,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long,
    signed char,
    signed short,
    signed int,
    signed long,
    signed long long
#endif
    >;
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Testing bounds_t constructors",
         .path             = "abc_test_test::utility::bounds",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_test_bounds = [&](abc::multi_element_test_block_t& _a_bba,
                                  const abc::utility::bounds_t<T>& _a_bounds,
                                  const T&                         _a_lower,
                                  const T&                         _a_upper,
                                  const std::size_t& _a_difference)
        {
            _a_bba += _BLOCK_CHECK(
                _EXPR(_a_lower == _a_bounds.lower())
                && _EXPR(_a_upper == _a_bounds.higher())
                && _EXPR(_a_difference == _a_bounds.difference())
            );
        };

        auto _l_type{typeid(T).name()};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format("Unit testing bounds_t constructors for {0}", _l_type)
        );
        using test_data_t = tuple<T, T, size_t>;
        for (const auto& _l_data : read_data_from_file<test_data_t>(
                 fmt::format("unit_tests_{0}", _l_type)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_first_val, _l_second_val, _l_difference]{_l_data};
            bounds_t<T> _l_bounds(_l_first_val, _l_second_val);
            const T&    _l_lower{
                _l_first_val < _l_second_val ? _l_first_val : _l_second_val
            };
            const T& _l_upper{
                _l_first_val < _l_second_val ? _l_second_val : _l_first_val
            };
            _l_test_bounds(
                _l_unit_tests, _l_bounds, _l_lower, _l_upper, _l_difference
            );
            _l_bounds = bounds_t(_l_second_val, _l_first_val);
            _l_test_bounds(
                _l_unit_tests, _l_bounds, _l_lower, _l_upper, _l_difference
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        bounds_t<T> _l_bounds;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property tests for bounds_t constructor using type {0}",
                _l_type
            )
        );
        // Default constructor tests
        _l_test_bounds(
            _l_property_tests,
            _l_bounds,
            numeric_limits<T>::min(),
            numeric_limits<T>::max(),
            static_cast<size_t>(numeric_limits<T>::max())
                - static_cast<size_t>(numeric_limits<T>::min())
        );
        using test_data_t = T;
        _ABC_NS_DG::data_generator_collection_t<test_data_t> _l_data_gen;
        if constexpr (sizeof(T) == 100)
        {
            _l_data_gen = enumerate_data<test_data_t>();
        }
        else
        {
            _l_data_gen = generate_data_randomly<test_data_t>();
        }
        for (const auto& _l_val : _l_data_gen)
        {
            _TVLOG(_l_val);
            bounds_t<T> _l_bounds(_l_val);
            _l_test_bounds(_l_property_tests, _l_bounds, _l_val, _l_val, 0);
        }

        using test_data_2_t = std::tuple<T, T>;
        _ABC_NS_DG::data_generator_collection_t<test_data_2_t> _l_data_gen_2;
        if constexpr (sizeof(T) == 100)
        {
            _l_data_gen_2 = enumerate_data<test_data_2_t>();
        }
        else
        {
            _l_data_gen_2 = generate_data_randomly<test_data_2_t>();
        }
        for (const auto& _l_data : _l_data_gen_2)
        {
            _TVLOG(_l_data);
            const auto& [_l_first_val, _l_second_val]{_l_data};
            bounds_t<T> _l_bounds(_l_first_val, _l_second_val);
            // Deduce lowest and highest values. Then find difference.
            const bool _l_first_val_smallest{_l_first_val < _l_second_val};
            const T&   _l_lower{
                _l_first_val_smallest ? _l_first_val : _l_second_val
            };
            const T& _l_upper{
                _l_first_val_smallest ? _l_second_val : _l_first_val
            };
            const size_t _l_difference{static_cast<size_t>(
                _l_first_val_smallest ? (_l_second_val - _l_first_val)
                                      : (_l_first_val - _l_second_val)
            )};
            _l_test_bounds(
                _l_property_tests, _l_bounds, _l_lower, _l_upper, _l_difference
            );
            // Run the same tests, but with the constructor calling the
            // arguments in the other order.
            _l_bounds = bounds_t<T>(_l_second_val, _l_first_val);
            _l_test_bounds(
                _l_property_tests, _l_bounds, _l_lower, _l_upper, _l_difference
            );
        }

        _END_BBA_CHECK(_l_property_tests);
    };
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Testing bounds_c concept",
         .path             = "abc_test_test::utility::bounds",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    file_based_map_t<u8string, bool> _l_data("unit_test");
    auto                             unit_test_func = [&]<typename T>()
    {
        using namespace abc::utility;

        using namespace abc;
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::detail::bounds_c<T> == _a_expected_result
                );
            }
        ));
    };
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (unit_test_func.operator()<T>()));
        }
    );
}