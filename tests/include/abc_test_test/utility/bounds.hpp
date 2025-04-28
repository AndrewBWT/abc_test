#pragma once
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

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for bounds_t constructors",
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
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_bounds_tests, "Testing hand-written values for bounds_t"
        );
        using test_data_t = tuple<T, T, T>;
        for (const auto& [_l_first_val, _l_second_val, _l_difference] :
            read_data_from_file<test_data_t>(fmt::format("bounds_t_handwritten_data_{0}",
                typeid(T).name())))
        {
            bounds_t<T>      _l_bounds(_l_first_val, _l_second_val);
            const T& _l_lower{
                _l_first_val < _l_second_val ? _l_first_val : _l_second_val
            };
            const T& _l_upper{
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
    };
    using container_type_list_t = type_list<
#if 0
        bool, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
#endif
        >;
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
        {.name = "Property tests for bounds_t constructors, using enumerated "
                 "and randomly generated values",
         .path = "abc_test_test::utility::bounds",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace abc::utility;
    auto _l_run_bounds_default_constructor = [&]<typename T>()
    {
        bounds_t<T> _l_bounds;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_bounds_tests,
            fmt::format(
                "Running default bounds_t constructor tests for type {0}",
                typeid(T)
            )
        );
        test_bounds(
            _l_bounds_tests,
            _l_bounds,
            numeric_limits<T>::max(),
            numeric_limits<T>::min(),
            numeric_limits<T>::max()
        );
        _END_BBA_CHECK(_l_bounds_tests);
    };
    auto _l_run_bounds_one_arg_constructor = [&]<typename T>()
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_bounds_tests,
            fmt::format(
                "Running single element bounds_t constructor tests for type "
                "{0}",
                typeid(T)
            )
        );
        using test_data_t = T;
        _ABC_NS_DG::data_generator_collection_t<test_data_t> _l_data_gen;
        if constexpr (sizeof(T) == 0)
        {
            _l_data_gen = enumerate_data<test_data_t>();
        }
        else
        {
            _l_data_gen = generate_data_randomly<test_data_t>();
        }
        for (const auto& _l_val : _l_data_gen)
        {
            bounds_t<T> _l_bounds(_l_val);
            test_bounds<T>(_l_bounds_tests, _l_bounds, _l_val, _l_val, 0);
        }
        _END_BBA_CHECK(_l_bounds_tests);
    };
    auto _l_bounds_constructor_two_args = [&]<typename T>()
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_bounds_tests,
            fmt::format(
                "Running two element bounds_t constructor tests for type {0}",
                typeid(T)
            )
        );
        using test_data_t = std::tuple<T, T>;
        _ABC_NS_DG::data_generator_collection_t<test_data_t> _l_data_gen;
        if constexpr (sizeof(T) == 1)
        {
            _l_data_gen = enumerate_data<test_data_t>();
        }
        else
        {
            _l_data_gen = generate_data_randomly<test_data_t>();
        }
        for (const auto& [_l_first_val, _l_second_val] : _l_data_gen)
        {
            bounds_t<T> _l_bounds(_l_first_val, _l_second_val);
            // Deduce lowest and highest values. Then find difference.
            const bool _l_first_val_smallest{_l_first_val < _l_second_val};
            const T&   _l_lower{
                _l_first_val_smallest ? _l_first_val : _l_second_val
            };
            const T& _l_upper{
                _l_first_val_smallest ? _l_second_val : _l_first_val
            };
            const T _l_difference{static_cast<T>(
                _l_first_val_smallest ? (_l_second_val - _l_first_val)
                                      : (_l_first_val - _l_second_val)
            )};
            test_bounds(
                _l_bounds_tests, _l_bounds, _l_upper, _l_lower, _l_difference
            );
            // Run the same tests, but with the constructor calling the
            // arguments in the other order.
            _l_bounds = bounds_t<T>(_l_second_val, _l_first_val);
            test_bounds(
                _l_bounds_tests, _l_bounds, _l_upper, _l_lower, _l_difference
            );
        }
        _END_BBA_CHECK(_l_bounds_tests);
    };
    auto _l_top_level_func = [&]<typename T>()
    {
        _l_run_bounds_default_constructor.operator()<T>();
        _l_run_bounds_one_arg_constructor.operator()<T>();
        _l_bounds_constructor_two_args.operator()<T>();
    };
    using container_type_list_t = type_list<
#if 0
        bool, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
#endif
        >;
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_top_level_func.operator()<T>()));
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
    using container_type_list_t = type_list<
#if 0
        bool,unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
#endif
        >;
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (unit_test_func.operator()<T>()));
        }
    );
}