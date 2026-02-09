#pragma once
#include "abc_test/core.hpp"
#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing the default_random_generator_t specializations for "
                 "the fundamental types",
         .path = "abc_test_test::stl_specializations::default_random_generator",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    using namespace _ABC_NS_UTILITY;
    using namespace _ABC_NS_DG;
    auto _l_create_def_rng
        = [&]<typename T>(
              const variant<pair<T, T>, T, monostate>& _a_constructor_data
          )
    {
        default_random_generator_t<T> _l_drg;
        switch (_a_constructor_data.index())
        {
        case 0:
        {
            auto _l_pair{std::get<0>(_a_constructor_data)};
            _l_drg
                = default_random_generator_t<T>(_l_pair.first, _l_pair.second);
        }
        break;
        case 1:
            _l_drg = default_random_generator_t<T>(get<1>(_a_constructor_data));
            break;
        default:
            break;
        }
        return _l_drg;
    };
    auto _l_unit_tests_func = [&]<typename T>()
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_unit_tests = _MULTI_MATCHER(
            fmt::format("Unit tests for type {0}", _l_type_name)
        );
        using unit_test_type_1 = tuple<
            size_t,
            rng_counter_t,
            size_t,
            T,
            // Representing the different constructors for bounds_t
            variant<pair<T, T>, T, monostate>>;
        for (auto&& [_l_rng_progress, _l_counter, _l_rng_after_call, _l_expected, _l_constructor_data] :
             read_data_from_file<unit_test_type_1>(
                 fmt::format("unit_tests_{0}_1", _l_type_name)
             ))
        {
            default_random_generator_t<T> _l_drg
                = _l_create_def_rng(_l_constructor_data);
            rng_t _l_rng = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
            _l_rng.progress(_l_rng_progress);
            const T _l_value_created{_l_drg.operator()(_l_rng, _l_counter)};
            _l_unit_tests << _CHECK(
                _EXPR(_l_rng_after_call == _l_rng.calls())
                && _EXPR(_l_expected == _l_value_created)
            );
        }
        _CHECK(_l_unit_tests);
    };
    auto _l_fuzzy_tests_func = [&]<typename T, typename Rng>(
                                   const std::size_t _a_max_progress_val
                                   = std::numeric_limits<size_t>::max()
                               )
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_fuzzy_tests = _MULTI_MATCHER(
            fmt::format("Fuzzy tests for type {0}", _l_type_name)
        );
        using fuzzy_test_type_1 = tuple<
            size_t,
            rng_counter_t,
            // Representing the different constructors for bounds_t
            variant<pair<T, T>, T, monostate>>;
        data_generator_collection_t<fuzzy_test_type_1> _l_rng
            = generate_data_randomly<fuzzy_test_type_1>(
                default_random_generator<fuzzy_test_type_1>(
                    default_random_generator<size_t>(0, _a_max_progress_val),
                    default_random_generator<rng_counter_t>(),
                    default_random_generator<variant<pair<T, T>, T, monostate>>(
                    )
                )
            );
        for (auto&& [_l_rng_progress, _l_counter, _l_constructor_data] : _l_rng)
        {
            default_random_generator_t<T> _l_drg
                = _l_create_def_rng(_l_constructor_data);
            rng_t _l_rng = rng_t::make_rng<Rng>(vector<uint32_t>());
            _l_rng.progress(_l_rng_progress);
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            const T _l_value_created{_l_drg.operator()(_l_rng, _l_counter)};
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests << _CHECK(_l_matcher);
        }
        _CHECK(_l_fuzzy_tests);
    };
    using data_types_t = abc::utility::type_list<
       /* signed char,
        unsigned char,
        signed short,
        unsigned short,
        signed int,
        unsigned int,
        signed long,
        unsigned long,
        signed long long,
        unsigned long long,
         bool,*/
        char
    //    wchar_t,
    //    char8_t,
    //    char16_t,
    //    char32_t
    >;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_unit_tests_func.operator()<T>()));
            RUN(_l_mdg,
                (_l_fuzzy_tests_func.operator(
                )<T, inner_rng_mt19937_64_t>(100'000)));
            RUN(_l_mdg, (_l_fuzzy_tests_func.operator()<T, simple_rng_t>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing the default_random_generator_t specializations for "
                 "fundamental types, where not all elements are valid",
         .path = "abc_test_test::stl_specializations::default_random_generator",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    using namespace _ABC_NS_UTILITY;
    using namespace _ABC_NS_DG;
    using namespace _ABC_NS_MATCHER;
    auto _l_within_bounds = [&]<typename T>(const T& _a_value) -> matcher_t
    {
        if constexpr (same_as<char, T>)
        {
            return true_matcher(fmt::format(u8"{0} is valid", _a_value));
        }
        else
        {
            return false_matcher(u8"Logic unwritten");
        }
    };
    auto _l_unit_tests_func = [&]<typename T>()
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_unit_tests = _MULTI_MATCHER(
            fmt::format("Unit tests for type {0}", _l_type_name)
        );
        using unit_test_type_1 = tuple<size_t, rng_counter_t, size_t, T>;
        for (auto&& [_l_rng_progress, _l_counter, _l_rng_after_call, _l_expected] :
             read_data_from_file<unit_test_type_1>(
                 fmt::format("unit_tests_{0}_1", _l_type_name)
             ))
        {
            default_random_generator_t<T> _l_drg(true);
            rng_t _l_rng = rng_t::make_rng<simple_rng_t>(vector<uint32_t>());
            _l_rng.progress(_l_rng_progress);
            const T _l_value_created{_l_drg.operator()(_l_rng, _l_counter)};
            _l_unit_tests << _CHECK(
                _EXPR(_l_rng_after_call == _l_rng.calls())
                && _EXPR(_l_expected == _l_value_created)
            );
        }
        _CHECK(_l_unit_tests);
    };
    auto _l_fuzzy_tests_func = [&]<typename T, typename Rng>(
                                   const std::size_t _a_max_progress_val
                                   = std::numeric_limits<size_t>::max()
                               )
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_fuzzy_tests = _MULTI_MATCHER(
            fmt::format("Fuzzy tests for type {0}", _l_type_name)
        );
        using fuzzy_test_type_1 = tuple<size_t, rng_counter_t>;
        data_generator_collection_t<fuzzy_test_type_1> _l_rng
            = generate_data_randomly<fuzzy_test_type_1>(
                default_random_generator<fuzzy_test_type_1>(
                    default_random_generator<size_t>(0, _a_max_progress_val),
                    default_random_generator<rng_counter_t>()
                )
            );
        for (auto&& [_l_rng_progress, _l_counter] : _l_rng)
        {
            default_random_generator_t<T> _l_drg(true);
            rng_t _l_rng = rng_t::make_rng<Rng>(vector<uint32_t>());
            _l_rng.progress(_l_rng_progress);
            matcher_t _l_matcher_1, _l_matcher_2;
            _BEGIN_NO_THROW_MATCHER(_l_matcher_1);
            const T _l_value_created{_l_drg.operator()(_l_rng, _l_counter)};
            _l_matcher_2 = _l_within_bounds(_l_value_created);
            _END_NO_THROW_MATCHER(_l_matcher_1);
            _l_fuzzy_tests << _CHECK(_l_matcher_1 && _l_matcher_2);
        }
        _CHECK(_l_fuzzy_tests);
    };
    using data_types_t = type_list<
        char 
        //wchar_t, char8_t, char16_t,
        //char32_t
        >;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_unit_tests_func.operator()<T>()));
            RUN(_l_mdg,
                (_l_fuzzy_tests_func.operator(
                )<T, inner_rng_mt19937_64_t>(100'000)));
            RUN(_l_mdg, (_l_fuzzy_tests_func.operator()<T, simple_rng_t>()));
        }
    );
}