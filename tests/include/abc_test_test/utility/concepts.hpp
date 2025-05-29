#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/concepts.hpp"

namespace
{
struct X
{};
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Tests for concepts has_back_inserter_c, "
           "has_front_inserter_c, has_unordered_inserter_c and has_inserter_c",
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
    using Value_Type   = int;
    using data_types_t = abc::utility::type_list<
        vector<Value_Type>,
        array<Value_Type, 3>,
        deque<Value_Type>,
        list<Value_Type>,
        forward_list<Value_Type>,
        set<Value_Type>,
        map<Value_Type, Value_Type>,
        multiset<Value_Type, Value_Type>,
        multimap<Value_Type, Value_Type>,
        unordered_set<Value_Type>,
        unordered_map<Value_Type, Value_Type>,
        unordered_multiset<Value_Type, Value_Type>,
        unordered_multimap<Value_Type, Value_Type>>;
    file_based_map_t<u8string, std::tuple<bool, bool, bool, bool>> _l_data(
        "unit_test"
    );
    auto _l_test_func = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const std::tuple<bool, bool, bool, bool>& _a_expected_results)
            {
                _TVLOG(_a_expected_results);
                auto& [_l_back_inserter, _l_front_inserter, _l_unordered_inserter, _l_inserter]{
                    _a_expected_results
                };
                return _EXPR(has_back_inserter_c<T> == _l_back_inserter)
                       && _EXPR(has_front_inserter_c<T> == _l_front_inserter)
                       && _EXPR(
                           has_unordered_inserter_c<T> == _l_unordered_inserter
                       )
                       && _EXPR(has_inserter_c<T> == _l_inserter);
            }
        ));
    };
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for has_append_range_c",
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
    file_based_map_t<pair<u8string, u8string>, bool> _l_data("unit_test");
    auto _l_append_range_test = [&]<typename T1, typename T2>()
    {
        auto _l_type_pair{make_pair(type_id<T1>(), type_id<T2>())};
        _TVLOG(_l_type_pair);
        _CHECK(_l_data.generate_matcher(
            _l_type_pair,
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    ( abc::utility::has_append_range_c<T1, T2> )
                    == _a_expected_result
                );
            }
        ));
    };
    using container_type_list_t = type_list<short, unsigned short>;
    auto _l_func1               = [&]<typename Value_Type>()
    {
        using container_type_list2 = type_list<
            array<Value_Type, 3>,
            vector<Value_Type>,
            deque<Value_Type>,
            list<Value_Type>,
            forward_list<Value_Type>,
            set<Value_Type>>;
        auto _l_func2 = [&]<typename R1>()
        {
            auto _l_func3 = [&]<typename Value_Type2>()
            {
                using container_type_list3 = type_list<
                    array<Value_Type2, 3>,
                    vector<Value_Type2>,
                    deque<Value_Type2>,
                    list<Value_Type2>,
                    forward_list<Value_Type2>,
                    set<Value_Type2>>;
                manual_data_generator_t _l_mdg;
                // Fourth level for loop.
                for_each_type<container_type_list3>(
                    [&]<typename R2>()
                    {
                        RUN(_l_mdg,
                            (_l_append_range_test.operator()<R1, R2>()));
                    }
                );
            };
            // Third level for loop
            manual_data_generator_t _l_mdg;
            for_each_type<container_type_list_t>(
                [&]<typename VT2>()
                {
                    RUN(_l_mdg, (_l_func3.operator()<VT2>()));
                }
            );
        };
        // Second level for loop.
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list2>(
            [&]<typename R1>()
            {
                RUN(_l_mdg, (_l_func2.operator()<R1>()));
            }
        );
    };
    // Top level for loop.
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename VT1>()
        {
            RUN(_l_mdg, (_l_func1.operator()<VT1>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Testing concepts from_chars_convertable_c and "
                             "to_chars_convertable_c",
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
    file_based_map_t<u8string, pair<bool, bool>> _l_data("unit_test");
    auto test_from_chars_convertable = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const pair<bool, bool>& _a_expected_result)
            {
                return _EXPR(
                           from_chars_convertable_c<T>
                           == _a_expected_result.first
                       )
                       && _EXPR(
                           to_chars_convertable_c<T>
                           == _a_expected_result.second
                       );
            }
        ));
    };
    using container_type_list_t = type_list<
        short int,
        unsigned short int,
        int,
        unsigned int,
        long int,
        unsigned long int,
        long long int,
        unsigned long long int,
        bool,
        signed char,
        unsigned char,
        char,
        wchar_t,
        char8_t,
        char16_t,
        char32_t,
        float,
        double,
        long double,
        X,
        vector<int>>;
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (test_from_chars_convertable.operator()<T>()));
        }
    );
}