#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/rng.hpp"
#include "abc_test_test/testing_utilities/simple_random_generator.hpp"
#include "abc_test_test/utility/rng/inner_rng_mt19937_64.hpp"
#include "abc_test_test/utility/rng/rng_utilities.hpp"

namespace
{
    template <typename T, typename F>
    inline void
        erorr_tests(
            abc::multi_element_test_block_t& _a_metb,
            const std::u8string_view         _a_str,
            F                                _a_func
        )
    {
        using namespace abc;
        using namespace abc::data_gen;
        using namespace std;
        using namespace utility;
        rng_t     _l_bad_rng{ rng_t::make_default_rng<T>() };
        _a_metb += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
            abc::unreachable_exception_t,
            u8"rng_t's inner random generator contains a nullptr. This is an "
            u8"invalid state for the rng_t object, and is undefined behaviour.",
            [&]()
            {
                abc::do_not_optimise(_a_func(_l_bad_rng));
            }
        ));
    }
} // namespace

_TEST_CASE(
    abc::test_case_t(
        { .name = "Tests for static make_rng functions and rng_t constructors",
         .path = "abc_test_test::utility::rng",
         .threads_required = 1 }
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    constexpr size_t _l_n_values_to_generate{ 10 };
    auto             _l_test_func = [&]<typename T>
    {
        if constexpr (same_as<T, simple_rng_t>)
        {
            _BEGIN_MULTI_ELEMENT_BBA(
                _l_unit_tests,
                "Unit tests for make_rng functions and rng_t constructors"
            );
            using unit_test_1 = std::tuple<seed_t, size_t>;
            for (const auto& _l_data :
                read_data_from_file<unit_test_1>("unit_test_1"))
            {
                _TVLOG_(_l_data);
                const auto& [_l_seed, _l_state] {_l_data};
                rng_t _l_rng{ rng_t::make_rng<T>(_l_seed) };
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(_l_rng.calls() == 0) && _EXPR(_l_rng() == _l_state)
                );
            }
            using unit_test_2 = tuple<seed_t, size_t, size_t>;
            for (const auto& _l_data :
                read_data_from_file<unit_test_2>("unit_test_2"))
            {
                _TVLOG_(_l_data);
                const auto& [_l_seed, _l_values_used_to_create_new_seed, _l_first_value_from_new_seed] {
                    _l_data
                    };
                rng_t _l_rng{ rng_t::make_rng<T>(_l_seed) };
                rng_t _l_new_rng{
                    _l_rng.make_rng(_l_values_used_to_create_new_seed)
                };

                _l_unit_tests += _BLOCK_CHECK(
                    // New rng has zero calls
                    _EXPR(_l_new_rng.calls() == 0)
                    // New rng's first value is as specified.
                    && _EXPR(_l_new_rng() == _l_first_value_from_new_seed)
                    // Old rng has had this number of calls made to it.
                    && _EXPR(
                        _l_rng.calls() == _l_values_used_to_create_new_seed
                    )
                );
            }

            using unit_test_3 = tuple<seed_t, size_t>;
            for (const auto& _l_data :
                read_data_from_file<unit_test_3>("unit_test_3")
                & generate_data_randomly<unit_test_3>())
            {
                _TVLOG_(_l_data);
                const auto& [_l_seed, _l_progress] {_l_data};
                rng_t _l_rng{ rng_t::make_rng<T>(_l_seed) };
                vector<std::mt19937_64::result_type> _l_values, _l_cmp_values;
                _l_rng.progress(_l_progress);
                for (size_t _l_idx{ 0 }; _l_idx < _l_n_values_to_generate;
                    ++_l_idx)
                {
                    _l_values.push_back(_l_rng());
                }
                auto _l_calls{ _l_rng.calls() };
                auto _l_next_rng{ _l_rng() };
                _l_rng.progress(_l_progress);
                rng_t _l_new_rng{ _l_rng };
                for (size_t _l_idx{ 0 }; _l_idx < _l_n_values_to_generate;
                    ++_l_idx)
                {
                    _l_cmp_values.push_back(_l_new_rng());
                }
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(_l_new_rng.calls() == _l_calls)
                    && _EXPR(_l_new_rng() == _l_next_rng)
                    && _EXPR(_l_cmp_values == _l_values)
                );
                _l_cmp_values.clear();
                rng_t _l_rng_2{ rng_t::make_rng<simple_rng_t>(_l_seed) };
                _l_rng_2.progress(_l_progress);
                for (size_t _l_idx{ 0 }; _l_idx < _l_n_values_to_generate;
                    ++_l_idx)
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
            const auto& [_l_seed] {_l_data};
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
            const auto& [_l_seed, _l_n_values_to_take] {_l_data};
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
            const auto& [_l_seed, _l_n_values_to_take_for_new_rng] {_l_data};
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            auto _l_rng{ rng_t::make_rng<T>(_l_seed) };
            do_not_optimise(_l_rng.make_rng(_l_n_values_to_take_for_new_rng));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        using test_4 = std::tuple<seed_t>;
        // The constructors which take an lvalue and rvalue of a rng_t object.
        for (const auto& _l_data : generate_data_randomly<test_4>())
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed] {_l_data};
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            auto _l_rng{ rng_t::make_rng<T>(_l_seed) };
            do_not_optimise(rng_t(_l_rng));
            do_not_optimise(rng_t(rng_t::make_rng<T>(_l_seed)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests, "Error tests checking unreachable behaviour"
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"make_rng",
            [](utility::rng_t& _a_rng)
            {
                return _a_rng.make_rng(10);
            }
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"Copy constructor",
            [](utility::rng_t& _a_rng)
            {
                return rng_t(_a_rng);
            }
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"Rvalue copy constructor",
            [](utility::rng_t& _a_rng)
            {
                return rng_t(rng_t(_a_rng));
            }
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"Assignment operator",
            [](utility::rng_t& _a_rng)
            {
                rng_t _l_cpy = _a_rng;
                return _l_cpy;
            }
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"RValue ssignment operator",
            [](utility::rng_t& _a_rng)
            {
                rng_t _l_cpy = rng_t(_a_rng);
                return _l_cpy;
            }
        );
        _END_BBA_CHECK(_l_error_tests);
    };

    manual_data_generator_t _l_mdg;
    using rng_type_list_t
        = abc::utility::type_list<simple_rng_t, inner_rng_mt19937_64_t>;
    for_each_type<rng_type_list_t>(
        [&]<typename T>()
    {
        RUN(_l_mdg, (_l_test_func.operator() < T > ()));
    }
    );
}

_TEST_CASE(
    abc::test_case_t(
        { .name = "Tests for rng_t::progress and operator()",
         .path = "abc_test_test::utility::rng",
         .threads_required = 1 }
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
        if constexpr (same_as<T, simple_rng_t>)
        {
            _BEGIN_MULTI_ELEMENT_BBA(
                _l_unit_tests, "Unit tests for operator() and progress"
            );
            using unit_test_1 = tuple<seed_t, size_t, size_t>;
            for (const auto& _l_data :
                read_data_from_file<unit_test_1>("unit_test_1"))
            {
                _TVLOG_(_l_data);
                const auto& [_l_seed, _l_progress, _l_expected_val] {_l_data};
                rng_t _l_rng{ rng_t::make_rng<T>(_l_seed) };
                _l_rng.progress(_l_progress);
                auto _l_rv{ _l_rng() };
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(_l_rng.calls() == (_l_progress + 1))
                    && _EXPR(_l_rv == _l_expected_val)
                );
            }
            using unit_test_2
                = tuple<seed_t, std::vector<std::pair<size_t, size_t>>>;
            for (const auto& _l_data :
                read_data_from_file<unit_test_2>("unit_test_2"))
            {
                _TVLOG_(_l_data);
                const auto& [_l_seed, _l_vect_progress_and_vals] {_l_data};
                rng_t _l_rng{ rng_t::make_rng<T>(_l_seed) };
                vector<pair<size_t, size_t>> _l_results, _l_expected_results;
                for (const auto& [_l_progress, _l_expected_val] :
                    _l_vect_progress_and_vals)
                {
                    _l_expected_results.push_back(
                        { _l_expected_val, _l_progress + 1 }
                    );
                    _l_rng.progress(_l_progress);
                    _l_results.push_back({ _l_rng(), _l_rng.calls() });
                }
                _l_unit_tests
                    += _BLOCK_CHECK(_EXPR(_l_expected_results == _l_results));
            }

            _END_BBA_CHECK(_l_unit_tests);
        }

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests, "Property tests for progress and operator ()"
        );
        using unit_test_1 = tuple<seed_t, size_t>;
        auto _l_rnd_generator{ default_random_generator_t<unit_test_1>() };
        get<1>(_l_rnd_generator.get_ref_to_inner_rngs())
            = default_random_generator<size_t>(0, _a_maximum_progress_allowed);
        for (const auto& _l_data : generate_data_randomly<unit_test_1>(
            mk_random_generator(_l_rnd_generator)
        ))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed, _l_progress_to_make] {_l_data};
            rng_t _l_rng{ rng_t::make_rng<simple_rng_t>(_l_seed) };
            rng_t _l_rng_2{ _l_rng };
            _l_rng.progress(_l_progress_to_make);
            for (size_t _l_idx{ 0 }; _l_idx < _l_progress_to_make; ++_l_idx)
            {
                _l_rng_2();
            }
            _l_property_tests += _BLOCK_CHECK(
                _EXPR(_l_rng() == _l_rng_2())
                && _EXPR(_l_rng.calls() == _l_rng_2.calls())
            );
        }
        _END_BBA_CHECK(_l_property_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_error_tests, "Error tests checking unreachable behaviour"
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"operator()",
            [](utility::rng_t& _a_rng)
            {
                return _a_rng.operator()();
            }
        );
        erorr_tests<T>(
            _l_error_tests,
            u8"progress",
            [](utility::rng_t& _a_rng)
            {
                _a_rng.progress(100);
                return _a_rng;
            }
        );
        _END_BBA_CHECK(_l_error_tests);
    };

    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator() < simple_rng_t > (100'000)));
    RUN(_l_mdg, (_l_func.operator() < inner_rng_mt19937_64_t > (100'000)));
}
_TEST_CASE(
    abc::test_case_t(
        { .name = "Testing all rng_t object's functions",
         .path = "abc_test_test::utility::rng",
         .threads_required = 1 }
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
        auto      _l_rnd_generator{ default_random_generator<unit_test_1>(
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
        ) };
        matcher_t _l_matcher;
        for (const auto& _l_data :
            generate_data_randomly<unit_test_1>(_l_rnd_generator))
        {
            _TVLOG_(_l_data);
            const auto& [_l_init, _l_actions] {_l_data};
            rng_t _l_rng = rng_t::make_rng<T>(seed_t());
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            if (auto _l_ptr{ get_if<pair<unsigned int, size_t>>(&_l_init) };
                _l_ptr != nullptr)
            {
                auto _l_args{ *_l_ptr };
                _l_rng = rng_t::make_rng<T>(_l_args.first, _l_args.second);
            }
            else if (auto _l_ptr{ get_if<seed_t>(&_l_init) }; _l_ptr != nullptr)
            {
                _l_rng = rng_t::make_rng<T>(*_l_ptr);
            }
            for (auto& _l_instr : _l_actions)
            {
                if (auto _l_ptr{ get_if<size_t>(&_l_instr) }; _l_ptr != nullptr)
                {
                    _l_rng.progress(*_l_ptr);
                }
                else if (auto _l_ptr{ get_if<monostate>(&_l_instr) };
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
    RUN(_l_mdg, (_l_func.operator() < simple_rng_t > ()));
    RUN(_l_mdg, _l_func.operator() < inner_rng_mt19937_64_t > (100'000));
}
