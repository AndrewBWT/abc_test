#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/concepts.hpp"

namespace
{
struct X
{};

template <typename F>
inline void
    unit_test_inserter_helper(
        F _a_function
    )
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
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
        vector<int>
#if 0

#endif
        >;
    auto _l_test_l1 = [&]<typename Value_Type>()
    {
        using container_type_list_vect_t = type_list<vector<Value_Type>
#if 0

#endif
                                                     >;
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_vect_t>(
            [&]<typename T>()
            {
                RUN(_l_mdg, (_a_function.operator()<T>()));
            }
        );
    };
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_l1.operator()<T>()));
        }
    );
    auto _l_test_pair_l1 = [&]<typename T1>()
    {
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list_t>(
            [&]<typename T2>()
            {
                RUN(_l_mdg, (_a_function.operator()<std::pair<T1, T2>>()));
            }
        );
    };
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_pair_l1.operator()<T>()));
        }
    );
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
    auto                             _l_test_l2 = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::has_back_inserter_c<T> == _a_expected_result
                );
            }
        ));
    };
    unit_test_inserter_helper(_l_test_l2);
}

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
    file_based_map_t<u8string, bool> _l_data("unit_test");
    auto                             _l_test_l2 = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::has_front_inserter_c<T> == _a_expected_result
                );
            }
        ));
    };
    unit_test_inserter_helper(_l_test_l2);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for has_unordered_inserter_c",
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
    auto                             _l_test_l2 = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::has_unordered_inserter_c<T>
                    == _a_expected_result
                );
            }
        ));
    };
    unit_test_inserter_helper(_l_test_l2);
}

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
    auto                             _l_test_l2 = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::has_inserter_c<T> == _a_expected_result
                );
            }
        ));
    };
    unit_test_inserter_helper(_l_test_l2);
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
    using container_type_list_t = type_list<short int>;
    auto _l_func1               = [&]<typename Value_Type>()
    {
        using container_type_list2 = type_list<vector<Value_Type>>;
        auto _l_func2              = [&]<typename R1>()
        {
            manual_data_generator_t _l_mdg;
            for_each_type<container_type_list2>(
                [&]<typename R2>()
                {
                    RUN(_l_mdg, (_l_append_range_test.operator()<R1, R2>()));
                }
            );
        };
        manual_data_generator_t _l_mdg;
        for_each_type<container_type_list2>(
            [&]<typename R1>()
            {
                RUN(_l_mdg, (_l_func2.operator()<R1>()));
            }
        );
    };
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_func1.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for from_chars_convertable_c",
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
    auto test_from_chars_convertable = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::from_chars_convertable_c<T>
                    == _a_expected_result
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
        vector<int>
#if 0

#endif
        >;
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (test_from_chars_convertable.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        { .name = "Unit tests for to_chars_convertable_c",
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
    auto test_to_chars_convertable = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::to_chars_convertable_c<T>
                    == _a_expected_result
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
        vector<int>
#if 0

#endif
    > ;
    manual_data_generator_t _l_mdg;
    for_each_type<container_type_list_t>(
        [&]<typename T>()
    {
        RUN(_l_mdg, (test_to_chars_convertable.operator() < T > ()));
    }
    );
}