#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp"
#include "abc_test/utility/bounds.hpp"
#include "abc_test/utility/enum.hpp"

namespace
{
enum class enum_with_enum_list_1 : uint32_t
{
    one,
    two,
    three,
    four,
    five
};
enum class enum_with_enum_list_2 : uint8_t
{
    one
};
enum class enum_with_enum_list_3 : int8_t
{
    One,
    Two,
    Three
};
enum class enum_no_enum_list
{
    one,
    two,
    three
};
enum class enum_with_enum_list_4
{
    one   = 1,
    two   = 2,
    three = 3,
    four  = 1
};
using type_list_t = abc::utility::type_list<
    enum_with_enum_list_1,
    enum_with_enum_list_2,
    enum_with_enum_list_3,
    enum_with_enum_list_4>;
} // namespace

namespace abc
{
template <>
inline auto
    utility::get_enum_list() noexcept -> utility::enum_list_t<enum_with_enum_list_1>
{
    using enum enum_with_enum_list_1;
    return {
        _ENUM_LIST_ENTRY(one), _ENUM_LIST_ENTRY(two), _ENUM_LIST_ENTRY(three)
    };
}

template <>
inline auto
    utility::get_enum_list() noexcept -> utility::enum_list_t<enum_with_enum_list_2>
{
    using enum enum_with_enum_list_2;
    return {_ENUM_LIST_ENTRY(one)};
}

template <>
inline auto
    utility::get_enum_list() noexcept -> utility::enum_list_t<enum_with_enum_list_3>
{
    using enum enum_with_enum_list_3;
    return {
        _ENUM_LIST_ENTRY(One), _ENUM_LIST_ENTRY(Two), _ENUM_LIST_ENTRY(Three)
    };
}

template <>
inline auto
    utility::get_enum_list() noexcept -> utility::enum_list_t<enum_with_enum_list_4>
{
    using enum enum_with_enum_list_4;
    return {
        _ENUM_LIST_ENTRY(one),
        _ENUM_LIST_ENTRY(two),
        _ENUM_LIST_ENTRY(three),
        _ENUM_LIST_ENTRY(four)
    };
}
} // namespace abc

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for enum_has_list_c concept",
         .path             = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    file_based_map_t<u8string, bool> _l_data("unit_test");
    auto                             _l_test_func = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::enum_has_list_c<T> == _a_expected_result
                );
            }
        ));
    };
    manual_data_generator_t _l_mdg;
    using type_list_local_t = abc::utility::type_list<
        enum_with_enum_list_1,
        enum_with_enum_list_2,
        enum_no_enum_list>;
    for_each_type<type_list_local_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Tests enumerate_enum_helper_t::less_than, equal and difference",
         .path             = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    // The lambda function operating over types.
    auto _l_test_func = [&]<typename Enum>()
    {
        auto                          _l_type_name{typeid(Enum).name()};
        enumerate_enum_helper_t<Enum> _l_helper(get_enum_list<Enum>());
        // Set up less than, equal and difference as function pointers
        auto _l_less_than = [&](const Enum& _a_enum1, const Enum& _a_enum2)
        {
            return _l_helper.less_than(_a_enum1, _a_enum2);
        };
        auto _l_equal = [&](const Enum& _a_enum1, const Enum& _a_enum2)
        {
            return _l_helper.equal(_a_enum1, _a_enum2);
        };
        auto _l_difference = [&](const Enum& _a_enum1, const Enum& _a_enum2)
        {
            return _l_helper.difference(_a_enum1, _a_enum2);
        };
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing less_than, equal and difference using {0}",
                _l_type_name
            )
        );
        using unit_test_data_1 = std::tuple<Enum, Enum, bool>;
        using unit_test_data_2 = std::tuple<Enum, Enum, size_t>;
        // lambda function operating over a binary function.
        auto unit_test_func    = [&]<typename Test_Type, typename F>(
                                  const string_view _a_name, F _a_func
                              )
        {
            for (const auto& _l_data : read_data_from_file<Test_Type>(
                     fmt::format("unit_tests_{0}_{1}", _a_name, _l_type_name)
                 ))
            {
                const auto& [_l_enum1, _l_enum2, _l_result]{_l_data};
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(_a_func(_l_enum1, _l_enum2) == _l_result)
                );
            }
        };
        // Unit testing less than.
        unit_test_func.template operator()<unit_test_data_1>("less_than", _l_less_than);
        // Unit testing equal.
        unit_test_func.template operator()<unit_test_data_1>("equal", _l_equal);
        // Unit testing difference.
        unit_test_func.template operator(
        )<unit_test_data_2>("difference", _l_difference);
        _END_BBA_CHECK(_l_unit_tests);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing less_than and equal using {0}", _l_type_name
            )
        );
        // Fuzzy test function.
        using fuzzy_test_data_t = std::tuple<Enum, Enum>;
        auto fuzzy_test_func
            = [&]<typename F>(const string_view _a_name, F _a_func)
        {
            for (const auto& _l_data : enumerate_data<fuzzy_test_data_t>())
            {
                const auto& [_l_enum1, _l_enum2]{_l_data};
                matcher_t _l_matcher;
                _BEGIN_NO_THROW_MATCHER(_l_matcher);
                do_not_optimise(_a_func(_l_enum1, _l_enum2));
                _END_NO_THROW_MATCHER(_l_matcher);
                _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
            }
        };
        fuzzy_test_func("less_than", _l_less_than);
        fuzzy_test_func("equal", _l_equal);
        fuzzy_test_func("difference", _l_difference);
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests,
            fmt::format(
                "Checking unreachable conditions on less_than, equal and "
                "difference for "
                "{0}",
                _l_type_name
            )
        );
        using Enum_underlying = std::underlying_type_t<Enum>;
        using error_test_data_t
            = std::tuple<Enum_underlying, Enum_underlying, u8string>;
        auto _l_error_func
            = [&]<typename F>(const string_view _a_name, F _a_func)
        {
            for (const auto& _l_data : read_data_from_file<error_test_data_t>(
                     fmt::format("error_tests_{0}_{1}", _a_name, _l_type_name)
                 ))
            {
                _TVLOG(_l_data);
                const auto& [_l_enum1_as_underlying, _l_enum2_as_underlying, _l_error]{
                    _l_data
                };
                auto _l_enum1{static_cast<Enum>(_l_enum1_as_underlying)};
                auto _l_enum2{static_cast<Enum>(_l_enum2_as_underlying)};
                _l_error_tests += _BLOCK_CHECK(
                    _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                        abc::unreachable_exception_t,
                        _l_error,
                        [&]()
                        {
                            do_not_optimise(_a_func(_l_enum1, _l_enum2));
                        }
                    )
                );
            }
        };
        _l_error_func("less_than", _l_less_than);
        _l_error_func("equal", _l_equal);
        _l_error_func("difference", _l_difference);
        _END_BBA_CHECK(_l_error_tests);
    };
    manual_data_generator_t _l_mdg;
    for_each_type<type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Tests for enumerate_enum_helper_t::min, max and size",
         .path = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename Enum>()
    {
        auto _l_type_name{typeid(Enum).name()};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing min, max and size using enum {0}", _l_type_name
            )
        );
        using test_data_t = std::tuple<Enum, Enum, size_t>;
        enumerate_enum_helper_t<Enum> _l_helper(get_enum_list<Enum>());
        for (const auto& _l_data : read_data_from_file<test_data_t>(
                 fmt::format("unit_tests_{0}", _l_type_name)
             ))
        {
            auto& [_l_min, _l_max, _l_size]{_l_data};

            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(_l_helper.min() == _l_min)
                && _EXPR(_l_helper.max() == _l_max)
                && _EXPR(_l_helper.size() == _l_size)
            );
        }
        _END_BBA_CHECK(_l_unit_tests);
    };
    manual_data_generator_t _l_mdg;
    for_each_type<type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Tests enumerate_enum_helper_t::increment and decrement",
         .path = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename Enum>()
    {
        auto _l_type_name{typeid(Enum).name()};
        auto _l_tn{cast_string_to_u8string(_l_type_name)};
        _TVLOG(_l_tn);
        enumerate_enum_helper_t<Enum> _l_helper(get_enum_list<Enum>());
        auto                          _l_increment =
            [&](Enum& _a_enum1, size_t& _a_size, const optional<Enum>& _a_limit)
        {
            return _l_helper.increment(_a_enum1, _a_size, _a_limit);
        };
        auto _l_decrement =
            [&](Enum& _a_enum1, size_t& _a_size, const optional<Enum>& _a_limit)
        {
            return _l_helper.decrement(_a_enum1, _a_size, _a_limit);
        };
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing increment and decrement using {0}", _l_type_name
            )
        );
        using unit_test_data_t
            = std::tuple<Enum, size_t, optional<Enum>, bool, Enum, size_t>;
        auto unit_test_func
            = [&]<typename F>(const string_view _a_name, F _a_func)
        {
            for (const auto& _l_data : read_data_from_file<unit_test_data_t>(
                     fmt::format("unit_tests_{0}_{1}", _a_name, _l_type_name)
                 ))
            {
                _TVLOG(_l_data);
                const auto& [_l_enum, _l_size, _l_limit, _l_bool_res, _l_enum_res, _l_size_res]{
                    _l_data
                };
                auto _l_enum_cpy{_l_enum};
                auto _l_size_cpy{_l_size};
                auto _l_res{_a_func(_l_enum_cpy, _l_size_cpy, _l_limit)};
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(_l_enum_cpy == _l_enum_res)
                    && _EXPR(_l_bool_res == _l_res)
                    && _EXPR(_l_size_cpy == _l_size_res)
                );
            }
        };
        unit_test_func("increment", _l_increment);
        unit_test_func("decrement", _l_decrement);
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing increment and decrement using {0}", _l_type_name
            )
        );
        using fuzzy_test_data_t = std::tuple<Enum, size_t, optional<Enum>>;
        auto _l_fuzzy_test =
            [&]<typename F>(
                const string_view _a_str, F _a_func, const bool _l_limit_higher
            )
        {
            _TVLOG(_l_limit_higher);
            using namespace abc::utility;
            using namespace abc::data_gen;
            // Create bespoke random generator.
            auto _l_rnd_func = using_function<fuzzy_test_data_t>(
                [&](rng_t& _a_rng, const rng_counter_t& _a_rng_counter)
                {
                    fuzzy_test_data_t _l_rv;
                    // Generate a random enum.
                    get<0>(_l_rv) = run_default_random_generator<Enum>(
                        _a_rng, _a_rng_counter
                    );
                    // If we are limited to only higher values, set the min to
                    // be the generated enum. Else the minimu possible value.
                    auto _l_lower{
                        _l_limit_higher ? get<0>(_l_rv)
                                        : min_value_t<Enum>{}.min_value()
                    };
                    // And set the max to the maximum possible value, else the
                    // generated enum.
                    auto _l_higher = _l_limit_higher
                                         ? max_value_t<Enum>{}.max_value()
                                         : get<0>(_l_rv);
                    // Generate a random size_t value.
                    get<1>(_l_rv)  = run_default_random_generator<size_t>(
                        _a_rng, _a_rng_counter
                    );
                    // Call the generator with the given arguments.
                    get<2>(_l_rv) = run_default_random_generator<Enum>(
                        _a_rng, _a_rng_counter, _l_lower, _l_higher
                    );
                    return _l_rv;
                }
            );
            for (const auto& _l_data :
                 generate_data_randomly<fuzzy_test_data_t>(_l_rnd_func))
            {
                _TVLOG(_l_data);
                const auto& [_l_enum, _l_size, _l_limit]{_l_data};
                auto      _l_enum_cpy{_l_enum};
                auto      _l_size_cpy{_l_size};
                matcher_t _l_matcher;
                _BEGIN_NO_THROW_MATCHER(_l_matcher);
                do_not_optimise(_a_func(_l_enum_cpy, _l_size_cpy, _l_limit));
                _END_NO_THROW_MATCHER(_l_matcher);
                _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
            }
        };
        _l_fuzzy_test("increment", _l_increment, true);
        _l_fuzzy_test("decrement", _l_decrement, false);
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property testing increment and decrement using {0}",
                _l_type_name
            )
        );
        using property_test_data_1 = std::tuple<Enum, Enum>;
        // Function for checking going forwards then backwards yields same enum.
        auto _l_property_test_func
            = [&]<typename F>(string_view _a_name, F _a_func)
        {
            auto _l_b_str = cast_string_to_u8string(_a_name);
            _TVLOG(_l_b_str);
            for (auto& _l_data : enumerate_data<property_test_data_1>())
            {
                _TVLOG(_l_data);
                auto& [_l_enum1, _l_enum2]{_l_data};
                auto& _l_lower{
                    _l_helper.less_than(_l_enum1, _l_enum2) ? _l_enum1
                                                            : _l_enum2
                };
                auto& _l_upper{
                    _l_helper.less_than(_l_enum1, _l_enum2) ? _l_enum2
                                                            : _l_enum1
                };
                const optional<Enum> _l_min_val{
                    _l_lower == _l_helper.min() ? nullopt
                                                : optional<Enum>{_l_lower}
                };
                const optional<Enum> _l_max_val{
                    _l_lower == _l_helper.max() ? nullopt
                                                : optional<Enum>{_l_upper}
                };
                // Now enumerate all enums between these values.
                for (auto& _l_enum_start :
                     enumerate_data<Enum>(from_m_to_n(_l_lower, _l_upper)))
                {
                    _TVLOG(_l_enum_start);
                    // Now generate some size_t values.
                    for (auto& _l_size :
                         generate_data_randomly<size_t>()
                             & iterate_over<size_t>(
                                 {numeric_limits<size_t>::max()}
                             )
                             // Enum
                             & enumerate_data<size_t>(
                                 from_min_to_val(_l_helper.size() + 10)
                             ))
                    {
                        _TVLOG(_l_size);
                        auto                   _l_enum_start_cpy{_l_enum_start};
                        auto                   _l_size_1{_l_size};
                        auto                   _l_size_2{_l_size};
                        const pair<bool, bool> _l_results{_a_func(
                            _l_enum_start_cpy,
                            _l_size_1,
                            _l_size_2,
                            _l_min_val,
                            _l_max_val
                        )};
                        _l_property_tests += _BLOCK_CHECK(
                            // Enums are same.
                            _EXPR(_l_enum_start_cpy == _l_enum_start)
                            // the second size == 0 and the second results
                            // == true.
                            && _EXPR(
                                _l_size_2 == 0
                                && _EXPR(_l_results.second == true)
                                // Either size_1 == 0 && result1 == true.
                                // Meaning it exhausted the integer.
                                && ((_EXPR(_l_size_1 == 0)
                                     && _EXPR(_l_results.first == true))
                                    // Or it didn't exhaust the integer.
                                    || (_EXPR(_l_size_1 > 0)
                                        && _EXPR(_l_results.first == false)))
                            )
                        );
                    }
                }
            }
        };
        _l_property_test_func(
            "increment then decrement",
            [&](Enum&                 _a_enum,
                size_t&               _a_size1,
                size_t&               _a_size2,
                const optional<Enum>& _a_min_val,
                const optional<Enum>& _a_max_val)
            {
                pair<bool, bool> _l_rv;
                _l_rv.first
                    = _l_helper.increment(_a_enum, _a_size1, _a_max_val);
                _a_size2     -= _a_size1;
                _l_rv.second  = _l_helper.decrement(_a_enum, _a_size2, nullopt);
                return _l_rv;
            }
        );
        _l_property_test_func(
            "decrement then increment",
            [&](Enum&                 _a_enum,
                size_t&               _a_size1,
                size_t&               _a_size2,
                const optional<Enum>& _a_min_val,
                const optional<Enum>& _a_max_val)
            {
                pair<bool, bool> _l_rv;
                _l_rv.first
                    = _l_helper.decrement(_a_enum, _a_size1, _a_min_val);
                _a_size2 -= _a_size1;
                _l_rv.second
                    = _l_helper.increment(_a_enum, _a_size2, std::nullopt);
                return _l_rv;
            }
        );

        _END_BBA_CHECK(_l_property_tests);


        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests,
            fmt::format(
                "Checking unreachable conditions on increment and decrement "
                "for "
                "{0}",
                _l_type_name
            )
        );
        using Enum_underlying = std::underlying_type_t<Enum>;
        // Tests that when an unregistered enum is used, it is UB.
        auto _l_error_func_1
            = [&]<typename F>(const string_view _a_name, F _a_func)
        {
            using error_test_data_t = std::
                tuple<Enum_underlying, optional<Enum_underlying>, u8string>;
            for (const auto& _l_data : read_data_from_file<error_test_data_t>(
                     fmt::format("error_tests_1_{0}_{1}", _a_name, _l_type_name)
                 ))
            {
                _TVLOG(_l_data);
                const auto& [_l_underlying_enum, _l_underlying_enum_limit, _l_error_msg]{
                    _l_data
                };
                auto           _l_enum{static_cast<Enum>(_l_underlying_enum)};
                optional<Enum> _l_enum_limit{
                    _l_underlying_enum_limit.has_value()
                        ? make_optional(static_cast<Enum>(
                              _l_underlying_enum_limit.value()
                          ))
                        : std::nullopt
                };
                _l_error_tests += _BLOCK_CHECK(
                    _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                        abc::unreachable_exception_t,
                        _l_error_msg,
                        [&]()
                        {
                            do_not_optimise(_a_func(_l_enum, _l_enum_limit));
                        }
                    )
                );
            }
        };
        _l_error_func_1(
            "increment_enum_invalid",
            [&](const Enum&           _a_enum,
                const optional<Enum>& _a_enum_limit) -> bool
            {
                Enum   _l_enum{_a_enum};
                size_t _l_size{0};
                return _l_helper.increment(_l_enum, _l_size, _a_enum_limit);
            }
        );
        _l_error_func_1(
            "decrement_enum_invalid",
            [&](const Enum&           _a_enum,
                const optional<Enum>& _a_enum_limit) -> bool
            {
                Enum   _l_enum{_a_enum};
                size_t _l_size{0};
                return _l_helper.decrement(_l_enum, _l_size, _a_enum_limit);
            }
        );

        auto _l_error_func_2
            = [&]<typename F>(const string_view _a_name, F _a_func)
        {
            using error_test_data_t = std::tuple<Enum, Enum, u8string>;
            for (const auto& _l_data : read_data_from_file<error_test_data_t>(
                     fmt::format("error_tests_2_{0}_{1}", _a_name, _l_type_name)
                 ))
            {
                const auto& [_l_enum, _l_enum_limit, _l_error_msg]{_l_data};
                _l_error_tests += _BLOCK_CHECK(
                    _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                        abc::unreachable_exception_t,
                        _l_error_msg,
                        [&]()
                        {
                            do_not_optimise(_a_func(_l_enum, _l_enum_limit));
                        }
                    )
                );
            }
        };
        _l_error_func_2(
            "increment_maxvalue_invalid",
            [&](const Enum& _a_enum, const Enum& _a_enum_limit)
            {
                Enum   _l_enum{_a_enum};
                size_t _l_size{0};
                return _l_helper.increment(_l_enum, _l_size, _a_enum_limit);
            }
        );
        _l_error_func_2(
            "decrement_maxvalue_invalid",
            [&](const Enum& _a_enum, const Enum& _a_enum_limit)
            {
                Enum   _l_enum{_a_enum};
                size_t _l_size{0};
                return _l_helper.decrement(_l_enum, _l_size, _a_enum_limit);
            }
        );
        _END_BBA_CHECK(_l_error_tests);
    };

    manual_data_generator_t _l_mdg;
    for_each_type<type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests enumerate_enum_helper_t::print and parse",
         .path             = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    auto _l_test_func = [&]<typename Enum>()
    {
        auto                          _l_type_name{typeid(Enum).name()};
        enumerate_enum_helper_t<Enum> _l_helper(get_enum_list<Enum>());
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing print and parse for the enum {0}", _l_type_name
            )
        );
        using unit_test_type_print = std::
            tuple<Enum, u8string, vector<optional<enum_helper_string_type_e>>>;
        for (auto& _l_data : read_data_from_file<unit_test_type_print>(
                 fmt::format("unit_test_print_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_enum, _l_expected, _l_str_types]{_l_data};
            for (auto& _l_str_type : _l_str_types)
            {
                _TVLOG(_l_str_type);
                u8string _l_result_print;
                if (_l_str_type.has_value())
                {
                    _l_result_print
                        = _l_helper.print(_l_enum, _l_str_type.value());
                }
                else
                {
                    _l_result_print = _l_helper.print(_l_enum);
                }
                _l_unit_tests
                    += _BLOCK_CHECK(_EXPR(_l_result_print == _l_expected));
            }
        }
        using unit_test_type_parse = std::tuple<
            result_t<Enum>,
            size_t,
            u8string,
            vector<optional<enum_helper_string_type_e>>>;
        for (auto& _l_data : read_data_from_file<unit_test_type_parse>(
                 fmt::format("unit_test_parse_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_expected_enum, _l_expected_elements_remaining, _l_str, _l_str_types]{
                _l_data
            };
            for (auto& _l_str_type : _l_str_types)
            {
                _TVLOG(_l_str_type);
                result_t<Enum>         _l_result_parse;
                parser::parser_input_t _l_pi(_l_str);
                if (_l_str_type.has_value())
                {
                    _l_result_parse
                        = _l_helper.parse_enum(_l_pi, _l_str_type.value());
                }
                else
                {
                    _l_result_parse = _l_helper.parse_enum(_l_pi);
                }
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(_l_result_parse == _l_expected_enum)
                    && _EXPR(
                        _l_pi.elements_processed()
                        == _l_expected_elements_remaining
                    )
                );
            }
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing print and parse for the enum {0}", _l_type_name
            )
        );
        using fuzzy_test_type_print
            = std::tuple<Enum, optional<enum_helper_string_type_e>>;
        for (auto& _l_data : enumerate_data<fuzzy_test_type_print>())
        {
            _TVLOG(_l_data);
            auto& [_l_enum, _l_str_type]{_l_data};
            u8string  _l_result_print;
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            if (_l_str_type.has_value())
            {
                _l_result_print = _l_helper.print(_l_enum, _l_str_type.value());
            }
            else
            {
                _l_result_print = _l_helper.print(_l_enum);
            }
            do_not_optimise(_l_result_print);
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        using fuzzy_test_type_parse = tuple<
            variant<Enum, u8string>,
            optional<enum_helper_string_type_e>>;
        for (auto& _l_data : generate_data_randomly<fuzzy_test_type_parse>())
        {
            _TVLOG(_l_data);
            auto& [_l_input, _l_str_type]{_l_data};
            result_t<Enum> _l_result_parse;
            u8string       _l_str_input;
            matcher_t      _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            switch (_l_input.index())
            {
            case 0:
                if (_l_str_type.has_value())
                {
                    _l_str_input = _l_helper.print(
                        get<0>(_l_input), _l_str_type.value()
                    );
                }
                else
                {
                    _l_str_input = _l_helper.print(get<0>(_l_input));
                }
                break;
            case 1:
                _l_str_input = get<1>(_l_input);
                break;
            default:
                break;
            }
            parser::parser_input_t _l_pi(_l_str_input);
            if (_l_str_type.has_value())
            {
                _l_result_parse
                    = _l_helper.parse_enum(_l_pi, _l_str_type.value());
            }
            else
            {
                _l_result_parse = _l_helper.parse_enum(_l_pi);
            }
            do_not_optimise(_l_result_parse);
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property testing print and parse for the enum {0}",
                _l_type_name
            )
        );
        using property_test_type
            = std::tuple<Enum, optional<enum_helper_string_type_e>>;
        for (auto& _l_data : enumerate_data<property_test_type>())
        {
            _TVLOG(_l_data);
            auto& [_l_enum, _l_str_type]{_l_data};
            result_t<Enum> _l_enum_result;
            if (_l_str_type.has_value())
            {
                parser::parser_input_t _l_pi(
                    _l_helper.print(_l_enum, _l_str_type.value())
                );
                _l_enum_result
                    = _l_helper.parse_enum(_l_pi, _l_str_type.value());
            }
            else
            {
                parser::parser_input_t _l_pi(_l_helper.print(_l_enum));
                _l_enum_result = _l_helper.parse_enum(_l_pi);
            }
            _l_property_tests += _BLOCK_CHECK(_EXPR(_l_enum_result == _l_enum));
        }
        _END_BBA_CHECK(_l_property_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests,
            fmt::format(
                "Error testing print and parse for the enum {0}", _l_type_name
            )
        );
        using enum_helper_string_type_underlying_e
            = std::underlying_type_t<enum_helper_string_type_e>;
        using enum_underlying_e     = std::underlying_type_t<Enum>;
        using error_test_type_print = std::tuple<
            enum_underlying_e,
            enum_helper_string_type_underlying_e,
            u8string>;
        for (auto& _l_data : read_data_from_file<error_test_type_print>(
                 fmt::format("error_test_print_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_enum_underlying, _l_str_type_underlying, _l_expected_error]{
                _l_data
            };
            Enum _l_enum{static_cast<Enum>(_l_enum_underlying)};
            enum_helper_string_type_e _l_str_type{
                static_cast<enum_helper_string_type_e>(_l_str_type_underlying)
            };
            _l_error_tests
                += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                    abc::unreachable_exception_t,
                    _l_expected_error,
                    [&]()
                    {
                        do_not_optimise(_l_helper.print(_l_enum, _l_str_type));
                    }
                ));
        }
        using error_test_type_parse = std::
            tuple<u8string, enum_helper_string_type_underlying_e, u8string>;
        for (auto& _l_data : read_data_from_file<error_test_type_parse>(
                 fmt::format("error_test_parse_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_str, _l_str_type_as_underlying, _l_expected_error]{_l_data
            };
            enum_helper_string_type_e _l_str_type{
                static_cast<enum_helper_string_type_e>(_l_str_type_as_underlying
                )
            };
            _l_error_tests
                += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                    abc::unreachable_exception_t,
                    _l_expected_error,
                    [&]()
                    {
                        parser::parser_input_t _l_pi(_l_str);
                        do_not_optimise(_l_helper.parse_enum(_l_pi, _l_str_type)
                        );
                    }
                ));
        }
        _END_BBA_CHECK(_l_error_tests);
    };

    manual_data_generator_t _l_mdg;
    for_each_type<type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Tests enumerate_enum_helper_t::enums_idx and idxs_enum",
         .path = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    auto _l_test_func = [&]<typename Enum>()
    {
        auto                          _l_type_name{typeid(Enum).name()};
        enumerate_enum_helper_t<Enum> _l_helper(get_enum_list<Enum>());
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing enums_idx and idxs_enum for the enum {0}",
                _l_type_name
            )
        );
        using unit_test_type = std::tuple<Enum, std::size_t>;
        for (auto& _l_data : read_data_from_file<unit_test_type>(
                 fmt::format("unit_tests_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_enum, _l_idx]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(_l_helper.enums_idx(_l_enum) == _l_idx)
                && _EXPR(_l_helper.idxs_enum(_l_idx) == _l_enum)
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing enums_idx and idxs_enum for the enum {0}",
                _l_type_name
            )
        );
        using fuzzy_test_type_enums_idx = Enum;
        for (auto& _l_data : enumerate_data<fuzzy_test_type_enums_idx>())
        {
            _TVLOG(_l_data);
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            auto _l_result{_l_helper.enums_idx(_l_data)};
            do_not_optimise(_l_result);
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }

        using fuzzy_test_type_idxs_enum = size_t;
        for (auto& _l_data : enumerate_data<fuzzy_test_type_idxs_enum>(
                 from_min_to_val(_l_helper.size() - 1)
             ))
        {
            _TVLOG(_l_data);
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            auto _l_result{_l_helper.idxs_enum(_l_data)};
            do_not_optimise(_l_result);
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property testing enums_idx and idxs_enum for the enum {0}",
                _l_type_name
            )
        );
        using property_test_type = Enum;
        for (auto& _l_data : enumerate_data<property_test_type>())
        {
            _TVLOG(_l_data);
            _l_property_tests += _BLOCK_CHECK(_EXPR(
                _l_helper.idxs_enum(_l_helper.enums_idx(_l_data)) == _l_data
            ));
        }
        _END_BBA_CHECK(_l_property_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests,
            fmt::format(
                "Error testing enums_idx and idxs_enum for the enum {0}",
                _l_type_name
            )
        );
        using enum_underlying_e = std::underlying_type_t<Enum>;
        using error_test_type_enums_idx
            = std::tuple<enum_underlying_e, u8string>;
        for (auto& _l_data : read_data_from_file<error_test_type_enums_idx>(
                 fmt::format("error_test_enums_idx_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_enum_underlying, _l_expected_error]{_l_data};
            Enum _l_enum{static_cast<Enum>(_l_enum_underlying)};
            _l_error_tests
                += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                    abc::unreachable_exception_t,
                    _l_expected_error,
                    [&]()
                    {
                        do_not_optimise(_l_helper.enums_idx(_l_enum));
                    }
                ));
        }
        using error_test_type_idxs_enum = std::tuple<size_t, u8string>;
        for (auto& _l_data : read_data_from_file<error_test_type_idxs_enum>(
                 fmt::format("error_test_idxs_enum_{0}", _l_type_name)
             ))
        {
            _TVLOG(_l_data);
            auto& [_l_size, _l_expected_error]{_l_data};
            _l_error_tests
                += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                    abc::unreachable_exception_t,
                    _l_expected_error,
                    [&]()
                    {
                        do_not_optimise(_l_helper.idxs_enum(_l_size));
                    }
                ));
        }
        _END_BBA_CHECK(_l_error_tests);
    };

    manual_data_generator_t _l_mdg;
    for_each_type<type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests enumerate_enum_helper_t constructor",
         .path             = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    auto _l_test_func = [&]<typename Enum>()
    {
        auto _l_type_name{typeid(Enum).name()};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing enumerate_enum_helper_t using valid enum_list "
                "for {0}",
                _l_type_name
            )
        );
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        enumerate_enum_helper_t<Enum> _l_helper(get_enum_list<Enum>());
        do_not_optimise(_l_helper);
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        _END_BBA_CHECK(_l_unit_tests);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests,
            fmt::format(
                "Error testing invalid enumerate_enum_helper_t constructors "
                "for {0}",
                _l_type_name
            )
        );
        using enum_underlying_e = std::underlying_type_t<Enum>;
        using error_test_type_constructor
            = tuple<vector<pair<enum_underlying_e, u8string>>, u8string>;
        for (auto& _l_data :
             read_data_from_file<error_test_type_constructor>(fmt::format(
                 "error_enumerate_enum_helper_t_constructor_{0}", _l_type_name
             )))
        {
            _TVLOG(_l_data);
            auto& [_l_enum_list, _l_error]{_l_data};
            enum_list_t<Enum> _l_arg;
            ranges::transform(
                _l_enum_list,
                back_inserter(_l_arg),
                [](const auto& _a_pair)
                {
                    return make_pair(
                        static_cast<Enum>(_a_pair.first),
                        u8string_view(_a_pair.second)
                    );
                }
            );
            _l_error_tests
                += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                    abc::errors::test_library_exception_t,
                    _l_error,
                    [&]()
                    {
                        enumerate_enum_helper_t<Enum> _l_helper(_l_arg);
                        do_not_optimise(_l_helper);
                    }
                ));
        }
        _END_BBA_CHECK(_l_error_tests);
    };

    using type_list = abc::utility::type_list<enum_with_enum_list_4>;
    manual_data_generator_t _l_mdg;
    for_each_type<type_list>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}