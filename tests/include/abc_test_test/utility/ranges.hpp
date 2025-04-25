#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/ranges.hpp"

namespace
{
struct X
{
    X() = delete;
};
} // namespace

namespace
{
template <typename T, typename U>
inline void
    test_static_cast_range_concept(
        abc::utility::io::file_based_map_t<std::u8string, bool>& _a_bfr
    )
{
    using namespace abc::utility;
    using namespace abc;
    _TVLOG(type_id<T>());
    _CHECK(_a_bfr.generate_matcher(
        type_id<T>(),
        [&](const bool& _a_expected_result)
        {
            return _EXPR(
                ( abc::utility::detail::static_cast_range_c<T, U> )
                == _a_expected_result
            );
        }
    ));
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for static_cast_range_c",
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
    file_based_map_t<u8string, bool> _l_data("unit_test");
    manual_data_generator_t          _l_mdg;
    RUN(_l_mdg,
        (test_static_cast_range_concept<vector<int>, vector<int>>(_l_data)));
    RUN(_l_mdg,
        (test_static_cast_range_concept<vector<char>, vector<unsigned char>>(
            _l_data
        )));
    RUN(_l_mdg,
        (test_static_cast_range_concept<set<char>, vector<char>>(_l_data)));
    RUN(_l_mdg,
        (test_static_cast_range_concept<set<char>, vector<unsigned char>>(
            _l_data
        )));
    RUN(_l_mdg,
        (test_static_cast_range_concept<set<X>, vector<vector<int>>>(_l_data)));
    RUN(_l_mdg,
        (test_static_cast_range_concept<list<int>, deque<char>>(_l_data)));
}

namespace
{
template <typename T>
inline void
    property_get_inserter_tests()
{
    using test_type_1 = std::tuple<T, std::ranges::range_value_t<T>>;
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
        _l_property_tests += _BLOCK_CHECK(_EXPR(_l_expected == _l_range_cpy));
    }
    _END_BBA_CHECK(_l_property_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Property tests for get_inserter",
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
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_get_inserter_tests<vector<int>>()));
    RUN(_l_mdg, (property_get_inserter_tests<vector<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<vector<unsigned char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<set<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<unordered_set<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<multiset<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<unordered_multiset<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<list<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<deque<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<forward_list<char>>()));
    //RUN(_l_mdg, (property_get_inserter_tests<map<char, int>>()));
    //RUN(_l_mdg, (property_get_inserter_tests<unordered_map<char, int>>()));
    //RUN(_l_mdg, (property_get_inserter_tests<multimap<char, int>>()));
   // RUN(_l_mdg, (property_get_inserter_tests<unordered_multimap<char, int>>()));
    RUN(_l_mdg, (property_get_inserter_tests<string>()));
    RUN(_l_mdg, (property_get_inserter_tests<u8string>()));
}

namespace
{
template <typename T, typename U, typename V>
inline void
    property_join_tests()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, fmt::format("Property tests for join")
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
        for (const auto& _l_element : std::ranges::to<vector<W>>(_l_range_1))
        {
            _l_hand_made_results_intermediary.push_back(_l_element);
        }
        for (const auto& _l_element : std::ranges::to<vector<W>>(_l_range_2))
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
}

template <typename Value_Type1, typename Value_Type2, typename T, typename U>
inline void
    property_join_pair_tests_l1()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_join_tests<map<Value_Type1, Value_Type2>, T, U>()));
    RUN(_l_mdg,
        (property_join_tests<unordered_map<Value_Type1, Value_Type2>, T, U>()));
    RUN(_l_mdg,
        (property_join_tests<multimap<Value_Type1, Value_Type2>, T, U>()));
    RUN(_l_mdg,
        (property_join_tests<
            unordered_multimap<Value_Type1, Value_Type2>,
            T,
            U>()));
}

template <typename Value_Type, typename T, typename U>
inline void
    property_join_tests_l1()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_join_tests<vector<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<vector<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<set<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<unordered_set<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<multiset<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<unordered_multiset<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<list<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<deque<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<forward_list<Value_Type>, T, U>()));
    RUN(_l_mdg, (property_join_tests<basic_string<Value_Type>, T, U>()));
}

template <typename Value_Type1, typename Value_Type2, typename U>
inline void
    property_join_pair_tests_l2()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (property_join_pair_tests_l1<
            Value_Type1,
            Value_Type2,
            map<Value_Type1, Value_Type2>,
            U>()));
    RUN(_l_mdg,
        (property_join_pair_tests_l1<
            Value_Type1,
            Value_Type2,
            unordered_map<Value_Type1, Value_Type2>,
            U>()));
    RUN(_l_mdg,
        (property_join_pair_tests_l1<
            Value_Type1,
            Value_Type2,
            multimap<Value_Type1, Value_Type2>,
            U>()));
    RUN(_l_mdg,
        (property_join_pair_tests_l1<
            Value_Type1,
            Value_Type2,
            unordered_multimap<Value_Type1, Value_Type2>,
            U>()));
}

template <typename Value_Type, typename U>
inline void
    property_join_tests_l2()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_join_tests_l1<Value_Type, vector<Value_Type>, U>()));
    RUN(_l_mdg, (property_join_tests_l1<Value_Type, vector<Value_Type>, U>()));
    RUN(_l_mdg, (property_join_tests_l1<Value_Type, set<Value_Type>, U>()));
    RUN(_l_mdg,
        (property_join_tests_l1<Value_Type, unordered_set<Value_Type>, U>()));
    RUN(_l_mdg,
        (property_join_tests_l1<Value_Type, multiset<Value_Type>, U>()));
    RUN(_l_mdg,
        (property_join_tests_l1<Value_Type, unordered_multiset<Value_Type>, U>()
        ));
    RUN(_l_mdg, (property_join_tests_l1<Value_Type, list<Value_Type>, U>()));
    RUN(_l_mdg, (property_join_tests_l1<Value_Type, deque<Value_Type>, U>()));
    RUN(_l_mdg,
        (property_join_tests_l1<Value_Type, forward_list<Value_Type>, U>()));
    RUN(_l_mdg,
        (property_join_tests_l1<Value_Type, basic_string<Value_Type>, U>()));
}

template <typename Value_Type1, typename Value_Type2>
inline void
    property_join_pair_tests_l3()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (property_join_pair_tests_l2<
            Value_Type1,
            Value_Type2,
            map<Value_Type1, Value_Type2>>()));
    RUN(_l_mdg,
        (property_join_pair_tests_l2<
            Value_Type1,
            Value_Type2,
            unordered_map<Value_Type1, Value_Type2>>()));
    RUN(_l_mdg,
        (property_join_pair_tests_l2<
            Value_Type1,
            Value_Type2,
            multimap<Value_Type1, Value_Type2>>()));
    RUN(_l_mdg,
        (property_join_pair_tests_l2<
            Value_Type1,
            Value_Type2,
            unordered_multimap<Value_Type1, Value_Type2>>()));
}

template <typename Value_Type>
inline void
    property_join_tests_l3()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (property_join_tests_l2<Value_Type, vector<Value_Type>>()));
    RUN(_l_mdg, (property_join_tests_l2<Value_Type, vector<Value_Type>>()));
    RUN(_l_mdg, (property_join_tests_l2<Value_Type, set<Value_Type>>()));
    RUN(_l_mdg,
        (property_join_tests_l2<Value_Type, unordered_set<Value_Type>>()));
    RUN(_l_mdg, (property_join_tests_l2<Value_Type, multiset<Value_Type>>()));
    RUN(_l_mdg,
        (property_join_tests_l2<Value_Type, unordered_multiset<Value_Type>>()));
    RUN(_l_mdg, (property_join_tests_l2<Value_Type, list<Value_Type>>()));
    RUN(_l_mdg, (property_join_tests_l2<Value_Type, deque<Value_Type>>()));
    RUN(_l_mdg,
        (property_join_tests_l2<Value_Type, forward_list<Value_Type>>()));
    RUN(_l_mdg,
        (property_join_tests_l2<Value_Type, basic_string<Value_Type>>()));
    /*if constexpr ()

    RUN(_l_mdg, (property_get_inserter_tests<deque<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<forward_list<char>>()));
    RUN(_l_mdg, (property_get_inserter_tests<map<char, int>>()));
    RUN(_l_mdg, (property_get_inserter_tests<unordered_map<char, int>>()));
    RUN(_l_mdg, (property_get_inserter_tests<multimap<char, int>>()));
    RUN(_l_mdg, (property_get_inserter_tests<unordered_multimap<char, int>>()));
    RUN(_l_mdg, (property_get_inserter_tests<string>()));
    RUN(_l_mdg, (property_get_inserter_tests<u8string>()));*/
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Property tests for join",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, property_join_tests_l3<int>());
    RUN(_l_mdg, (property_join_pair_tests_l3<int, char>()));
}

namespace
{
template <typename T, typename U>
inline void
    fuzzy_static_cast_range_type_tests()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, fmt::format("Fuzzy static_cast_range tests")
    );
    using test_1 = U;
    // for make_rng using a seed_t
    auto _l_rnd_gen{default_random_generator<test_1>()};
    for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
    {
        _TVLOG_(_l_data);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(static_cast_range<T>(_l_data));
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for static_cast_range",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (fuzzy_static_cast_range_type_tests<vector<char>, vector<signed char>>()
        ));
    RUN(_l_mdg,
        (fuzzy_static_cast_range_type_tests<vector<uint8_t>, vector<char>>()));
    RUN(_l_mdg,
        (fuzzy_static_cast_range_type_tests<string, vector<signed char>>()));
    RUN(_l_mdg,
        (fuzzy_static_cast_range_type_tests<vector<signed char>, u8string>()));
    RUN(_l_mdg,
        (fuzzy_static_cast_range_type_tests<set<signed char>, u8string>()));
}

namespace
{
template <typename T, typename U>
inline void
    property_static_cast_range_type_tests()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_property_tests, fmt::format("Fuzzy static_cast_range tests")
    );
    using test_1 = U;
    // for make_rng using a seed_t
    auto _l_rnd_gen{default_random_generator<test_1>()};
    for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
    {
        _TVLOG_(_l_data);
        auto _l_result = static_cast_range<T>(_l_data);
        vector<ranges::range_value_t<T>> _l_expected_vector;
        for (const auto& _l_element : _l_data)
        {
            _l_expected_vector.push_back(
                static_cast<ranges::range_value_t<T>>(_l_element)
            );
        }
        auto _l_expected{std::ranges::to<T>(_l_expected_vector)};
        _l_property_tests += _BLOCK_CHECK(_EXPR(_l_expected == _l_result));
    }
    _END_BBA_CHECK(_l_property_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Property tests for static_cast_range",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (property_static_cast_range_type_tests<
            vector<char>,
            vector<signed char>>()));
    RUN(_l_mdg,
        (property_static_cast_range_type_tests<vector<uint8_t>, vector<char>>())
    );
}