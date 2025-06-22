#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy tests for inner_rng_mtd19937_64",
         .path = "abc_test_test::utility::rng::inner_rng_mt19937_64",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, fmt::format("Fuzzy tests for inner_rng_mtd19937_64")
    );
    using test_1 = pair<
        // How to construct the object.
        variant<seed_t, std::mt19937_64>,
        // The various functions that can be called on the object.
        vector<
            variant<size_t, seed_t, std::monostate, std::monostate, seed_t>>>;
    auto _l_rnd_gen{default_random_generator<test_1>(
        default_random_generator<variant<seed_t, mt19937_64>>(),
        default_random_generator<
            vector<variant<size_t, seed_t, monostate, monostate, seed_t>>>(
            default_random_generator<
                variant<size_t, seed_t, monostate, monostate, seed_t>>(
                default_random_generator<size_t>(0, 100'000),
                default_random_generator<seed_t>(),
                default_random_generator<monostate>(),
                default_random_generator<monostate>(),
                default_random_generator<seed_t>()
            )
        )
    )};
    for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
    {
        _TVLOG_(_l_data);
        const auto& [_l_constructor, _l_instrs]{_l_data};
        matcher_t _l_matcher;
        _l_matcher.add_source_info(
            "_BEGIN_NO_THROW_MATCHER",
            abc::utility::str::mk_str_representing_function_args("_l_matcher"),
            std::source_location::current()
        );
        _l_matcher = abc::no_exception_thrown<true>();
        try
        {
            ;
            inner_rng_mt19937_64_t _l_rng = inner_rng_mt19937_64_t(seed_t());
            switch (_l_constructor.index())
            {
            case 0:
                _l_rng = inner_rng_mt19937_64_t(get<0>(_l_constructor));
                break;
            case 1:
                _l_rng = inner_rng_mt19937_64_t(get<1>(_l_constructor));
                break;
            }
            for (auto& _l_instr : _l_instrs)
            {
                switch (_l_instr.index())
                {
                case 0:
                    _l_rng.progress(get<0>(_l_instr));
                    break;
                case 1:
                    _l_rng.set_seed(get<1>(_l_instr));
                    break;
                case 2:
                    _l_rng();
                    break;
                case 3:
                    _l_rng.deep_copy();
                    break;
                case 4:
                    _l_rng.make_rng(get<4>(_l_instr));
                    break;
                }
            }
            do_not_optimise(_l_rng());
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    }

    _TEST_CASE(abc::test_case_t(
        {.name = "Testing inner_rng_mtd19937_64::set_seed",
         .path = "abc_test_test::utility::rng::inner_rng_mt19937_64",
         .threads_required = 1}
    ))
    {
        using namespace abc;
        using namespace abc::data_gen;
        using namespace std;
        using namespace utility;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests, fmt::format("Fuzzy tests for set_seed")
        );
        using test_1
            = tuple<mt19937_64, seed_t, vector<variant<monostate, size_t>>>;
        auto _l_rnd_gen = default_random_generator<test_1>(
            default_random_generator<mt19937_64>(),
            default_random_generator<seed_t>(),
            default_random_generator<vector<variant<monostate, size_t>>>(
                default_random_generator<variant<monostate, size_t>>(
                    default_random_generator<monostate>(),
                    default_random_generator<size_t>(0, 100'000)
                )
            )
        );
        for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
        {
            _TVLOG_(_l_data);
            const auto& [_l_mtd, _l_seed, _l_instrs]{_l_data};
            inner_rng_mt19937_64_t _l_rnd(_l_mtd);
            _l_rnd.set_seed(_l_seed);
            inner_rng_mt19937_64_t          _l_rnd2(_l_seed);
            vector<mt19937_64::result_type> _l_rng_1, _l_rng_2;
            for (const auto& _l_instr : _l_instrs)
            {
                switch (_l_instr.index())
                {
                case 0:
                    _l_rng_1.push_back(_l_rnd());
                    _l_rng_2.push_back(_l_rnd2());
                    break;
                case 1:
                    _l_rnd.progress(get<1>(_l_instr));
                    _l_rnd2.progress(get<1>(_l_instr));
                    break;
                }
            }
            _l_fuzzy_tests += _BLOCK_CHECK(_EXPR(_l_rnd() == _l_rnd2()));
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    }

    _TEST_CASE(abc::test_case_t(
        {.name = "Testing inner_rng_mtd19937_64::progress and operator()",
         .path = "abc_test_test::utility::rng::inner_rng_mt19937_64",
         .threads_required = 1}
    ))
    {
        using namespace abc;
        using namespace abc::data_gen;
        using namespace std;
        using namespace utility;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format("Property test for progress and operator()")
        );
        using test_1    = tuple<seed_t, size_t>;
        auto _l_rnd_gen = default_random_generator<test_1>(
            default_random_generator<seed_t>(),
            default_random_generator<size_t>(0, 100'000)
        );
        for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed, _l_progress]{_l_data};
            inner_rng_mt19937_64_t  _l_rnd_1(_l_seed);
            inner_rng_mt19937_64_t  _l_rnd_2(_l_seed);
            mt19937_64::result_type _l_result_1, _l_result_2;
            for (size_t _l_idx{0}; _l_idx < _l_progress; ++_l_idx)
            {
                _l_rnd_1();
            }
            _l_rnd_2.progress(_l_progress);
            _l_property_tests += _BLOCK_CHECK(_EXPR(_l_rnd_1() == _l_rnd_2()));
        }
        _END_BBA_CHECK(_l_property_tests);
    }

    _TEST_CASE(abc::test_case_t(
        {.name = "Testing inner_rng_mtd19937_64::deep_copy",
         .path = "abc_test_test::utility::rng::inner_rng_mt19937_64",
         .threads_required = 1}
    ))
    {
        using namespace abc;
        using namespace abc::data_gen;
        using namespace std;
        using namespace utility;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests, fmt::format("Property test for deep_copy")
        );
        using test_1    = tuple<seed_t>;
        auto _l_rnd_gen = default_random_generator<test_1>();
        for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed1]{_l_data};
            inner_rng_mt19937_64_t _l_rnd_1(_l_seed1);
            auto                   _l_cpy{_l_rnd_1.deep_copy()};
            _l_property_tests
                += _BLOCK_CHECK(_EXPR(_l_rnd_1() == _l_cpy->operator()()));
        }
        _END_BBA_CHECK(_l_property_tests);
    }

    _TEST_CASE(abc::test_case_t(
        {.name = "Testing inner_rng_mtd19937_64::make_rng",
         .path = "abc_test_test::utility::rng::inner_rng_mt19937_64",
         .threads_required = 1}
    ))
    {
        using namespace abc;
        using namespace abc::data_gen;
        using namespace std;
        using namespace utility;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests, fmt::format("Property test for make_rng")
        );
        using test_1    = tuple<seed_t, seed_t>;
        auto _l_rnd_gen = default_random_generator<test_1>();
        for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
        {
            _TVLOG_(_l_data);
            const auto& [_l_seed1, _l_seed2]{_l_data};
            inner_rng_mt19937_64_t _l_rnd_1(_l_seed1);
            inner_rng_mt19937_64_t _l_rnd_2(_l_seed2);
            auto                   _l_cpy{_l_rnd_2.make_rng(_l_seed1)};
            _l_property_tests
                += _BLOCK_CHECK(_EXPR(_l_rnd_1() == _l_cpy->operator()()));
        }
        _END_BBA_CHECK(_l_property_tests);
    }