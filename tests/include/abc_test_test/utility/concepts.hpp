#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/concepts.hpp"

namespace
{
template <typename T>
inline void
    test_back_inserter(
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
                abc::utility::has_back_inserter_c<T> == _a_expected_result
            );
        }
    ));
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for has_back_inserter_c",
         .path             = "abc_test_test::utility::concepts",
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
    RUN(_l_mdg, (test_back_inserter<array<int, 3>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<vector<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<deque<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<forward_list<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<list<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<set<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<unordered_set<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<unordered_map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<unordered_multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<unordered_multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_back_inserter<basic_string<int>>(_l_data)));
}

namespace
{
template <typename T>
inline void
    test_front_inserter(
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
                abc::utility::has_front_inserter_c<T> == _a_expected_result
            );
        }
    ));
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for has_front_inserter_c",
         .path             = "abc_test_test::utility::concepts",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    file_based_map_t<std::u8string, bool> _l_data("unit_test");
    manual_data_generator_t               _l_mdg;
    RUN(_l_mdg, (test_front_inserter<array<int, 3>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<vector<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<deque<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<forward_list<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<list<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<set<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<unordered_set<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<unordered_map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<unordered_multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<unordered_multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_front_inserter<basic_string<int>>(_l_data)));
}

namespace
{
template <typename T>
inline void
    test_inserter(
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
            return _EXPR(abc::utility::has_inserter_c<T> == _a_expected_result);
        }
    ));
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for has_inserter_c",
         .path             = "abc_test_test::utility::concepts",
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
    RUN(_l_mdg, (test_inserter<array<int, 3>>(_l_data)));
    RUN(_l_mdg, (test_inserter<vector<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<deque<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<forward_list<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<list<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<set<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<unordered_set<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<unordered_map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<unordered_multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<unordered_multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_inserter<basic_string<int>>(_l_data)));
}

namespace
{
template <typename T>
inline void
    test_sized_and_resizable_range(
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
                abc::utility::sized_and_reservable_range_c<T>
                == _a_expected_result
            );
        }
    ));
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for sized_and_reservable_range_c",
         .path             = "abc_test_test::utility::concepts",
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
    RUN(_l_mdg, (test_sized_and_resizable_range<array<int, 3>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<vector<int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<deque<int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<forward_list<int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<list<int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<set<int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<map<int, int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<multiset<int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<multimap<int, int>>(_l_data)));
    RUN(_l_mdg, (test_sized_and_resizable_range<unordered_set<int>>(_l_data)));
    RUN(_l_mdg,
        (test_sized_and_resizable_range<unordered_map<int, int>>(_l_data)));
    RUN(_l_mdg,
        (test_sized_and_resizable_range<unordered_multiset<int>>(_l_data)));
    RUN(_l_mdg,
        (test_sized_and_resizable_range<unordered_multimap<int, int>>(_l_data))
    );
    RUN(_l_mdg, (test_sized_and_resizable_range<basic_string<int>>(_l_data)));
}

namespace
{
    template <typename T>
    inline void
        test_from_chars_convertable(
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
                    abc::utility::from_chars_convertable_c<T>
                    == _a_expected_result
                );
            }
        ));
    }
} // namespace

_TEST_CASE(
    abc::test_case_t(
        { .name = "Unit tests for from_chars_convertable_c",
         .path = "abc_test_test::utility::concepts",
         .threads_required = 1 }
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
    RUN(_l_mdg, (test_from_chars_convertable<short int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<unsigned short int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<unsigned int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<long int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<unsigned long int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<long long int>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<unsigned long long int>(_l_data)));

    RUN(_l_mdg, (test_from_chars_convertable<bool>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<signed char>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<unsigned char>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<char>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<wchar_t>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<char16_t>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<char32_t>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<char8_t>(_l_data)));

    RUN(_l_mdg, (test_from_chars_convertable<float>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<double>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<long double>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<X>(_l_data)));
    RUN(_l_mdg, (test_from_chars_convertable<vector<int>>(_l_data)));
}