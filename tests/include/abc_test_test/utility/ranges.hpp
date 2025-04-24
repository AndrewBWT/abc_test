#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/ranges.hpp"

namespace
{
template <typename T>
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
    using test_1 = pair<vector<T>, vector<T>>;
    // for make_rng using a seed_t
    auto _l_rnd_gen{default_random_generator<test_1>()};
    for (const auto& _l_data : generate_data_randomly<test_1>(_l_rnd_gen))
    {
        _TVLOG_(_l_data);
        const auto& [_l_vect1, _l_vect2]{_l_data};
        vector<T> _l_result{abc::utility::join(_l_vect1, _l_vect2)};
        vector<T> _l_hand_made_results;
        for (const auto& _l_element : _l_vect1)
        {
            _l_hand_made_results.push_back(_l_element);
        }
        for (const auto& _l_element : _l_vect2)
        {
            _l_hand_made_results.push_back(_l_element);
        }
        _l_fuzzy_tests
            += _BLOCK_CHECK(_EXPR(_l_hand_made_results == _l_result));
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
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
    RUN(_l_mdg, property_join_tests<int>());
    RUN(_l_mdg, property_join_tests<bool>());
    RUN(_l_mdg, (property_join_tests<vector<char>>()));
}

namespace
{
struct X
{
    X() = delete;
};
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for range_statically_convertable_c",
         .path             = "abc_test_test::utility::ranges",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    _CHECK_EXPR(
        ( abc::utility::detail::
              statically_convertable_range_c<vector<int>, vector<int>> )
        == true
    );
    _CHECK_EXPR(
        ( abc::utility::detail::statically_convertable_range_c<
            vector<char>,
            vector<unsigned char>> )
        == true
    );
    _CHECK_EXPR(
        ( abc::utility::detail::
              statically_convertable_range_c<set<char>, vector<unsigned char>> )
        == false
    );
    _CHECK_EXPR(
        ( abc::utility::detail::
              statically_convertable_range_c<vector<X>, vector<X>> )
        == false
    );
    _CHECK_EXPR(
        ( abc::utility::detail::statically_convertable_range_c<int, vector<X>> )
        == false
    );
    _CHECK_EXPR(
        ( abc::utility::detail::
              statically_convertable_range_c<vector<int>, vector<vector<int>>> )
        == false
    );
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
        vector<ranges::range_value_t<U>> _l_expected_vector;
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