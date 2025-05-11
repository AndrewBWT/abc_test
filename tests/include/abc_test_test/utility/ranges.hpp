#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/ranges.hpp"

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <utility>

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for get_inserter",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    auto test_inerter_property_test = [&]<typename T, typename Insert_Type>()
    {
        using test_type_1 = std::tuple<T, Insert_Type>;
        using namespace abc;
        using namespace abc::utility;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests, fmt::format("Property tests for get_iterator")
        );
        for (const auto& _l_data : generate_data_randomly<test_type_1>())
        {
            const auto& [_l_range, _l_element]{_l_data};
            auto _l_range_cpy{_l_range};
            auto _l_inserter{abc::utility::detail::get_inserter(_l_range_cpy)};
            _l_inserter      = _l_element;
            auto _l_expected = _l_range;
            if constexpr (has_back_inserter_c<T>)
            {
                _l_expected.push_back(_l_element);
            }
            else if constexpr (has_unordered_inserter_c<T>)
            {
                _l_expected.insert(_l_element);
            }
            else if constexpr (has_front_inserter_c<T>)
            {
                _l_expected.push_front(_l_element);
            }
            _l_property_tests
                += _BLOCK_CHECK(_EXPR(_l_expected == _l_range_cpy));
        }
        _END_BBA_CHECK(_l_property_tests);
    };
    auto test_runner_top_level = [&]<typename T>()
    {
        // unordered_set and unordered_multiset need a std::hash for pairs,
        // these do not exist in the std.
        using container_type_list_t = type_list<
            vector<T>,
            deque<T>,
            forward_list<T>,
            list<T>,
            set<T>,
            multiset<T>,
            unordered_set<T>,
            unordered_multiset<T>,
            basic_string<T>>;
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T1>()
            {
                RUN(_l_mdg, (test_inerter_property_test.operator()<T1, T>()));
            }
        );
    };
    auto pair_test_runner_top_level
        = [&]<typename Value_Type1, typename Value_Type2>()
    {
        // unordered_set and unordered_multiset need a std::hash for pairs,
        // these do not exist in the std.
        using container_type_list_t = type_list<
            map<Value_Type1, Value_Type2>,
            unordered_map<Value_Type1, Value_Type2>,
            multimap<Value_Type1, Value_Type2>,
            unordered_multimap<Value_Type1, Value_Type2>,
            vector<pair<Value_Type1, Value_Type2>>,
            set<pair<Value_Type1, Value_Type2>>,
            // unordered_set<pair<Value_Type1, Value_Type2>>,
            multiset<pair<Value_Type1, Value_Type2>>,
            // unordered_multiset<pair<Value_Type1, Value_Type2>>,
            list<pair<Value_Type1, Value_Type2>>,
            deque<pair<Value_Type1, Value_Type2>>,
            forward_list<pair<Value_Type1, Value_Type2>>>;
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T1>()
            {
                RUN(_l_mdg,
                    (test_inerter_property_test.operator(
                    )<T1, pair<Value_Type1, Value_Type2>>()));
            }
        );
    };

    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (test_runner_top_level.operator()<int>()));
    RUN(_l_mdg, (test_runner_top_level.operator()<char>()));
    RUN(_l_mdg, (test_runner_top_level.operator()<unsigned char>()));
    RUN(_l_mdg, (pair_test_runner_top_level.operator()<char, int>()));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for append_range",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    auto property_append_range_test = [&]<typename T, typename U>()
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Property tests for append_range using {0}, and {1}",
                typeid(T).name(),
                typeid(U).name()
            )
        );
        using test_1 = pair<T, U>;
        using W      = std::ranges::range_value_t<T>;
        // for make_rng using a seed_t
        auto _l_rnd_gen{default_random_generator<test_1>()};
        for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
        {
            _TVLOG_(_l_data);
            const auto& [_l_range_1, _l_range_2]{_l_data};
            auto _l_range1_cpy{_l_range_1};
            abc::utility::detail::append_range(_l_range1_cpy, _l_range_2);
            vector<W> _l_hand_made_results_intermediary;
            for (const auto& _l_element :
                 std::ranges::to<vector<W>>(_l_range_1))
            {
                _l_hand_made_results_intermediary.push_back(_l_element);
            }
            for (const auto& _l_element :
                 std::ranges::to<vector<W>>(_l_range_2))
            {
                _l_hand_made_results_intermediary.push_back(_l_element);
            }
            T _l_hand_made_results{
                std::ranges::to<T>(_l_hand_made_results_intermediary)
            };
            _l_fuzzy_tests
                += _BLOCK_CHECK(_EXPR(_l_hand_made_results == _l_range1_cpy));
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    auto property_append_range_tests_top_level = [&]<typename Value_Type>()
    {
        using container_type_list_t = type_list<
            vector<Value_Type>,
            set<Value_Type>,
            unordered_set<Value_Type>,
            multiset<Value_Type>,
            unordered_multiset<Value_Type>,
            list<Value_Type>,
            deque<Value_Type>,
            forward_list<Value_Type>,
            basic_string<Value_Type>>;
        auto function_l1 = [&]<typename T2>()
        {
            manual_data_generator_t _l_mdg;
            for_each_type<container_type_list_t>(
                [&]<typename T1>()
                {
                    RUN(_l_mdg,
                        (property_append_range_test.operator()<T1, T2>()));
                }
            );
        };
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T2>()
            {
                RUN(_l_mdg, (function_l1.operator()<T2>()));
            }
        );
    };
    // Pairs test runner.
    auto property_append_range_tests_for_pairs_top_level
        = [&]<typename Value_Type1, typename Value_Type2>()
    {
        // unordered_set and unordered_multiset need a std::hash for pairs,
        // these do not exist in the std.
        using container_type_list_t = type_list<
            map<Value_Type1, Value_Type2>,
            unordered_map<Value_Type1, Value_Type2>,
            multimap<Value_Type1, Value_Type2>,
            unordered_multimap<Value_Type1, Value_Type2>,
            vector<pair<Value_Type1, Value_Type2>>,
            set<pair<Value_Type1, Value_Type2>>,
            // unordered_set<pair<Value_Type1, Value_Type2>>,
            multiset<pair<Value_Type1, Value_Type2>>,
            // unordered_multiset<pair<Value_Type1, Value_Type2>>,
            list<pair<Value_Type1, Value_Type2>>,
            deque<pair<Value_Type1, Value_Type2>>,
            forward_list<pair<Value_Type1, Value_Type2>>>;
        auto function_l1 = [&]<typename T2>()
        {
            manual_data_generator_t _l_mdg;
            for_each_type<container_type_list_t>(
                [&]<typename T1>()
                {
                    RUN(_l_mdg,
                        (property_append_range_test.operator()<T1, T2>()));
                }
            );
        };
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T2>()
            {
                RUN(_l_mdg, (function_l1.operator()<T2>()));
            }
        );
    };

    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_append_range_tests_top_level.operator()<int>()));
    RUN(_l_mdg,
        (property_append_range_tests_for_pairs_top_level.operator()<int, char>()
        ));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for join",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    auto property_join_test = [&]<typename T, typename U, typename V>()
    {
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Property tests for join using {0}, {1} and {2}",
                typeid(T).name(),
                typeid(U).name(),
                typeid(V).name()
            )
        );
        using test_1 = pair<U, V>;
        using W      = std::ranges::range_value_t<T>;
        // for make_rng using a seed_t
        auto _l_rnd_gen{default_random_generator<test_1>()};
        for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
        {
            _TVLOG_(_l_data); 
            const auto& [_l_range_1, _l_range_2]{_l_data};
            T         _l_result{abc::utility::join<T>(_l_range_1, _l_range_2)};
            vector<W> _l_hand_made_results_intermediary;
            for (const auto& _l_element :
                 std::ranges::to<vector<W>>(_l_range_1))
            {
                _l_hand_made_results_intermediary.push_back(_l_element);
            }
            for (const auto& _l_element :
                 std::ranges::to<vector<W>>(_l_range_2))
            {
                _l_hand_made_results_intermediary.push_back(_l_element);
            }
            T _l_hand_made_results{
                std::ranges::to<T>(_l_hand_made_results_intermediary)
            };
            _l_fuzzy_tests
                += _BLOCK_CHECK(_EXPR(_l_hand_made_results == _l_result));
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    auto property_join_tests_top_level = [&]<typename Value_Type>()
    {
        using container_type_list_t = type_list<
            vector<Value_Type>,
            set<Value_Type>,
            unordered_set<Value_Type>,
            multiset<Value_Type>,
            unordered_multiset<Value_Type>,
            list<Value_Type>,
            deque<Value_Type>,
            forward_list<Value_Type>>;
        using container_type_list2_t= type_list<
            vector<Value_Type>,
            set<Value_Type>,
            unordered_set<Value_Type>,
            multiset<Value_Type>,
            unordered_multiset<Value_Type>,
            list<Value_Type>,
            deque<Value_Type>,
            forward_list<Value_Type>>;
        auto function_l1 = [&]<typename T3>()
        {
            auto function_l2 = [&]<typename T2>()
            {
                manual_data_generator_t _l_mdg;
                for_each_type<container_type_list2_t>(
                    [&]<typename T1>()
                    {
                        RUN(_l_mdg,
                            (property_join_test.operator()<T1, T2, T3>()));
                    }
                );
            };

            manual_data_generator_t _l_mdg;
            for_each_type<container_type_list_t>(
                [&]<typename T2>()
                {
                    RUN(_l_mdg, (function_l2.operator()<T2>()));
                }
            );
        };
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T3>()
            {
                RUN(_l_mdg, (function_l1.operator()<T3>()));
            }
        );
    };
    // Pairs test runner.
    auto property_join_tests_for_pairs_top_level
        = [&]<typename Value_Type1, typename Value_Type2>()
    {
        // unordered_set and unordered_multiset need a std::hash for pairs,
        // these do not exist in the std.
        using container_type_list_t = type_list<
            map<Value_Type1, Value_Type2>,
            unordered_map<Value_Type1, Value_Type2>,
            multimap<Value_Type1, Value_Type2>,
            unordered_multimap<Value_Type1, Value_Type2>,
            vector<pair<Value_Type1, Value_Type2>>,
            set<pair<Value_Type1, Value_Type2>>,
            // unordered_set<pair<Value_Type1, Value_Type2>>,
            multiset<pair<Value_Type1, Value_Type2>>,
            // unordered_multiset<pair<Value_Type1, Value_Type2>>,
            list<pair<Value_Type1, Value_Type2>>,
            deque<pair<Value_Type1, Value_Type2>>,
            forward_list<pair<Value_Type1, Value_Type2>>>;
        using container_type_list_2t = type_list<
            map<Value_Type1, Value_Type2>,
            unordered_map<Value_Type1, Value_Type2>,
            multimap<Value_Type1, Value_Type2>,
            unordered_multimap<Value_Type1, Value_Type2>,
            vector<pair<Value_Type1, Value_Type2>>,
            set<pair<Value_Type1, Value_Type2>>,
            // unordered_set<pair<Value_Type1, Value_Type2>>,
            multiset<pair<Value_Type1, Value_Type2>>,
            // unordered_multiset<pair<Value_Type1, Value_Type2>>,
            list<pair<Value_Type1, Value_Type2>>,
            deque<pair<Value_Type1, Value_Type2>>,
            forward_list<pair<Value_Type1, Value_Type2>>>;
        auto function_l1 = [&]<typename T3>()
        {
            auto funtion_l2 = [&]<typename T2>()
            {
                manual_data_generator_t _l_mdg;
                for_each_type<container_type_list_2t>(
                    [&]<typename T1>()
                    {
                        RUN(_l_mdg,
                            (property_join_test.operator()<T1, T2, T3>()));
                    }
                );
            };

            manual_data_generator_t _l_mdg;
            for_each_type<container_type_list_t>(
                [&]<typename T2>()
                {
                    RUN(_l_mdg, (funtion_l2.operator()<T2>()));
                }
            );
        };
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T3>()
            {
                RUN(_l_mdg, (function_l1.operator()<T3>()));
            }
        );
    };

    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_join_tests_top_level.operator()<int>()));
    RUN(_l_mdg,
        (property_join_tests_for_pairs_top_level.operator()<int, char>()));
}