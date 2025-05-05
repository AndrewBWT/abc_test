#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/rng.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"
#include "abc_test_test/utility/rng/inner_rng_mt19937_64.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for static make_rng functions",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for static make_rng functions"
    );
    using unit_test_1 = std::tuple<seed_t, size_t>;
    for (const auto& _l_data : read_data_from_file<unit_test_1>("unit_test_1"))
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed, _l_state]{_l_data};
        rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed)};
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(_l_rng.calls() == 0) && _EXPR(_l_rng() == _l_state)
        );
    }
    using unit_test_2 = std::tuple<unsigned int, size_t, size_t>;
    for (const auto& _l_data : read_data_from_file<unit_test_2>("unit_test_2"))
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed, _l_n_values_to_take, _l_state]{_l_data};
        rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed, _l_n_values_to_take)
        };
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(_l_rng.calls() == 0) && _EXPR(_l_rng() == _l_state)
        );
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for all rng_t's constructors",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    auto _l_func = [&]<typename T>()
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy tests for constructors of rng_t<{0}> objects",
                typeid(T).name()
            )
        );
        using test_1 = std::tuple<seed_t>;
        // for make_rng using a seed_t
        matcher_t _l_matcher;
        for (const auto& _l_data : generate_data_randomly<test_1>())
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed]{_l_data};
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(rng_t::make_rng<T>(_l_seed));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        using test_2 = std::tuple<unsigned int, size_t>;
        // For static make_rng using an unsigned int and the number of values to
        // take.
        for (const auto& _l_data :
             generate_data_randomly<test_2>(default_random_generator<test_2>(
                 default_random_generator<unsigned int>(),
                 default_random_generator<size_t>(0, 100'000)
             )))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed, _l_n_values_to_take]{_l_data};
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(rng_t::make_rng<T>(_l_seed, _l_n_values_to_take));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        using test_3 = std::tuple<seed_t, size_t>;
        // For the make_rng constructor which uses a currently existing rng to
        // create a new rng.
        for (const auto& _l_data :
             generate_data_randomly<test_3>(default_random_generator<test_3>(
                 default_random_generator<seed_t>(),
                 default_random_generator<size_t>(0, 100'000)
             )))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed, _l_n_values_to_take_for_new_rng]{_l_data};
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            auto _l_rng{rng_t::make_rng<T>(_l_seed)};
            do_not_optimise(_l_rng.make_rng(_l_n_values_to_take_for_new_rng));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        using test_4 = std::tuple<seed_t>;
        // The constructors which take an lvalue and rvalue of a rng_t object.
        for (const auto& _l_data : generate_data_randomly<test_4>())
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed]{_l_data};
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            auto _l_rng{rng_t::make_rng<T>(_l_seed)};
            do_not_optimise(rng_t(_l_rng));
            do_not_optimise(rng_t(rng_t::make_rng<T>(_l_seed)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator()<simple_rng_t>()));
    RUN(_l_mdg, (_l_func.operator()<inner_rng_mt19937_64_t>()));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for make_rng function",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(_l_unit_tests, "Unit tests for make_rng function");
    using unit_test_1 = tuple<seed_t, size_t, size_t>;
    for (const auto& _l_data : read_data_from_file<unit_test_1>("unit_test_1"))
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed, _l_values_used_to_create_new_seed, _l_first_value_from_new_seed]{
            _l_data
        };
        rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed)};
        rng_t _l_new_rng{_l_rng.make_rng(_l_values_used_to_create_new_seed)};

        _l_unit_tests += _BLOCK_CHECK(
            // New rng has zero calls
            _EXPR(_l_new_rng.calls() == 0)
            // New rng's first value is as specified.
            && _EXPR(_l_new_rng() == _l_first_value_from_new_seed)
            // Old rng has had this number of calls made to it.
            && _EXPR(_l_rng.calls() == _l_values_used_to_create_new_seed)
        );
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests for rng constructors takeing an rng_t object as "
                 "an argument",
         .path = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    constexpr size_t _l_n_values_to_generate{10};
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for rng_t constructors"
    );
    using unit_test_1 = tuple<seed_t, size_t>;
    for (const auto& _l_data : read_data_from_file<unit_test_1>("unit_test_1")
                                   & generate_data_randomly<unit_test_1>())
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed, _l_progress]{_l_data};
        rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed)};
        vector<std::mt19937_64::result_type> _l_values, _l_cmp_values;
        _l_rng.progress(_l_progress);
        for (size_t _l_idx{0}; _l_idx < _l_n_values_to_generate; ++_l_idx)
        {
            _l_values.push_back(_l_rng());
        }
        auto _l_calls{_l_rng.calls()};
        auto _l_next_rng{_l_rng()};
        _l_rng.progress(_l_progress);
        rng_t _l_new_rng{_l_rng};
        for (size_t _l_idx{0}; _l_idx < _l_n_values_to_generate; ++_l_idx)
        {
            _l_cmp_values.push_back(_l_new_rng());
        }
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(_l_new_rng.calls() == _l_calls)
            && _EXPR(_l_new_rng() == _l_next_rng)
            && _EXPR(_l_cmp_values == _l_values)
        );
        _l_cmp_values.clear();
        rng_t _l_rng_2{rng_t::make_rng<simple_rng_t>(_l_seed)};
        _l_rng_2.progress(_l_progress);
        for (size_t _l_idx{0}; _l_idx < _l_n_values_to_generate; ++_l_idx)
        {
            _l_cmp_values.push_back(_l_rng_2());
        }
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(_l_rng_2.calls() == _l_calls)
            && _EXPR(_l_rng_2() == _l_next_rng)
            && _EXPR(_l_cmp_values == _l_values)
        );
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for operator()",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    constexpr size_t _l_n_values_to_generate{1'000};
    _BEGIN_MULTI_ELEMENT_BBA(_l_unit_tests, "Unit tests for operator ()");
    using unit_test_1 = tuple<seed_t, size_t, size_t>;
    for (const auto& _l_data : read_data_from_file<unit_test_1>("unit_test_1"))
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed, _l_progress, _l_expected_val]{_l_data};
        rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed)};
        _l_rng.progress(_l_progress);
        auto _l_rv{_l_rng()};
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(_l_rng.calls() == (_l_progress + 1))
            && _EXPR(_l_rv == _l_expected_val)
        );
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for progress",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(_l_unit_tests, "Unit tests for progress");
    using unit_test_1 = tuple<seed_t, std::vector<std::pair<size_t, size_t>>>;
    for (const auto& _l_data : read_data_from_file<unit_test_1>("unit_test_1"))
    {
        _TVLOG_(_l_data);
        const auto& [_l_seed, _l_vect_progress_and_vals]{_l_data};
        rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed)};
        vector<pair<size_t, size_t>> _l_results, _l_expected_results;
        for (const auto& [_l_progress, _l_expected_val] :
             _l_vect_progress_and_vals)
        {
            _l_expected_results.push_back({_l_expected_val, _l_progress + 1});
            _l_rng.progress(_l_progress);
            _l_results.push_back({_l_rng(), _l_rng.calls()});
        }
        _l_unit_tests += _BLOCK_CHECK(_EXPR(_l_expected_results == _l_results));
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Property tests for progress and operator ()",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    auto _l_func = [&]<typename T>(
                       const std::size_t _a_maximum_progress_allowed
                       = std::numeric_limits<std::size_t>::max()
                   )
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests, "Property tests for progress and operator ()"
        );
        using unit_test_1 = tuple<seed_t, size_t>;
        auto _l_rnd_generator{default_random_generator_t<unit_test_1>()};
        get<1>(_l_rnd_generator.get_ref_to_inner_rngs())
            = default_random_generator<size_t>(0, _a_maximum_progress_allowed);
        for (const auto& _l_data : generate_data_randomly<unit_test_1>(
                 mk_random_generator(_l_rnd_generator)
             ))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed, _l_progress_to_make]{_l_data};
            rng_t _l_rng{rng_t::make_rng<simple_rng_t>(_l_seed)};
            rng_t _l_rng_2{_l_rng};
            _l_rng.progress(_l_progress_to_make);
            for (size_t _l_idx{0}; _l_idx < _l_progress_to_make; ++_l_idx)
            {
                _l_rng_2();
            }
            _l_property_tests += _BLOCK_CHECK(
                _EXPR(_l_rng() == _l_rng_2())
                && _EXPR(_l_rng.calls() == _l_rng_2.calls())
            );
        }
        _END_BBA_CHECK(_l_property_tests);
    };
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator()<simple_rng_t>(100'000)));
    RUN(_l_mdg, (_l_func.operator()<inner_rng_mt19937_64_t>(100'000)));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for rng_t",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    auto _l_func = [&]<typename T>(
                       const std::size_t _a_maximum_progress
                       = std::numeric_limits<size_t>::max()
                   )
    {
        _BEGIN_MULTI_ELEMENT_BBA(_l_fuzzy_tests, "Fuzzy rng_t tests");
        // The first element represents the constructor.
        //  The second element represents a list of instructions.
        // size_t (1st) represents "progress".
        // monostate (2nd) represents operator().
        using unit_test_1 = tuple<
            variant<pair<unsigned int, size_t>, seed_t>,
            vector<variant<size_t, monostate>>>;
        auto _l_rnd_generator{default_random_generator<unit_test_1>(
            default_random_generator<
                variant<pair<unsigned int, size_t>, seed_t>>(
                default_random_generator<pair<unsigned int, size_t>>(
                    default_random_generator<unsigned int>(),
                    // Limit constructor to take first 100k values to create
                    // rng.
                    default_random_generator<size_t>(0, 100'000)
                ),
                default_random_generator<seed_t>()
            ),
            default_random_generator<vector<variant<size_t, monostate>>>(
                default_random_generator<variant<size_t, monostate>>(
                    // Maximum progress is how far we can progress at once.
                    // Very high numbers for this value will cause slowdown.
                    default_random_generator<size_t>(0, _a_maximum_progress),
                    default_random_generator<monostate>()
                )
            )
        )};
        matcher_t _l_matcher;
        for (const auto& _l_data :
             generate_data_randomly<unit_test_1>(_l_rnd_generator))
        {
            _TVLOG_(_l_data);
            const auto& [_l_init, _l_actions]{_l_data};
            rng_t _l_rng = rng_t::make_rng<T>(seed_t());
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            if (auto _l_ptr{get_if<pair<unsigned int, size_t>>(&_l_init)};
                _l_ptr != nullptr)
            {
                auto _l_args{*_l_ptr};
                _l_rng = rng_t::make_rng<T>(_l_args.first, _l_args.second);
            }
            else if (auto _l_ptr{get_if<seed_t>(&_l_init)}; _l_ptr != nullptr)
            {
                _l_rng = rng_t::make_rng<T>(*_l_ptr);
            }
            for (auto& _l_instr : _l_actions)
            {
                if (auto _l_ptr{get_if<size_t>(&_l_instr)}; _l_ptr != nullptr)
                {
                    _l_rng.progress(*_l_ptr);
                }
                else if (auto _l_ptr{get_if<monostate>(&_l_instr)};
                         _l_ptr != nullptr)
                {
                    _l_rng();
                }
            }
            do_not_optimise(_l_rng());
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator()<simple_rng_t>()));
    RUN(_l_mdg, _l_func.operator()<inner_rng_mt19937_64_t>(100'000));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests checking unreachable behaviour",
         .path             = "abc_test_test::utility::rng",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    auto _l_func = [&]<typename T>(
                       const std::size_t _a_maximum_progress
                       = std::numeric_limits<size_t>::max()
                   )
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests, "Unit tests checking unreachable behaviour"
        );
        matcher_t _l_matcher;
        rng_t _l_bad_rng{ rng_t::make_default_rng<T>() };
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        do_not_optimise(_l_bad_rng.make_rng(10));
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher, abc::unreachable_exception_t, u8"nullptr not correct");
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);

        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        _l_bad_rng.progress(10);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher, abc::unreachable_exception_t, u8"nullptr not correct");
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);

        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        do_not_optimise(_l_bad_rng.operator()());
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher, abc::unreachable_exception_t, u8"nullptr not correct");
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);

        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        do_not_optimise(rng_t(_l_bad_rng));
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher, abc::unreachable_exception_t, u8"nullptr not correct");
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);

        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        rng_t _l_new_rng = _l_bad_rng;
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher, abc::unreachable_exception_t, u8"nullptr not correct");
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        
        _END_BBA_CHECK(_l_unit_tests);
    };
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator()<simple_rng_t>()));
    RUN(_l_mdg, _l_func.operator()<inner_rng_mt19937_64_t>(100'000));
}