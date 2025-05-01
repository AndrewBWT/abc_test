#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp"
#include "abc_test/utility/bounds.hpp"
#include "abc_test/utility/enum.hpp"

namespace
{
enum class enum_with_enum_list_1 : uint32_t
{
    one,
    two,
    three,
    four,
    five
};

enum class enum_no_enum_list
{
    one,
    two,
    three
};
} // namespace

namespace abc
{
template <>
inline auto
    utility::get_enum_list() -> utility::enum_list_t<enum_with_enum_list_1>
{
    using enum enum_with_enum_list_1;
    return {
        _ENUM_LIST_ENTRY(one), _ENUM_LIST_ENTRY(two), _ENUM_LIST_ENTRY(three)
    };
}
} // namespace abc

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for enum_has_list_c concept",
         .path             = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    file_based_map_t<u8string, bool> _l_data("unit_test");
    auto                             _l_test_func = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const bool& _a_expected_result)
            {
                return _EXPR(
                    abc::utility::enum_has_list_c<T> == _a_expected_result
                );
            }
        ));
    };
    using type_list_t = type_list<enum_with_enum_list_1, enum_no_enum_list>;
    manual_data_generator_t _l_mdg;
    for_each_type<type_list_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests enumerate_enum_helper_t::less_than, difference and equal",
         .path = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    _BEGIN_MULTI_ELEMENT_BBA(_l_unit_tests, "Unit testing less_than, difference and equal");
    using test_data_t
        = std::tuple<enum_with_enum_list_1, enum_with_enum_list_1, bool, bool, std::size_t>;
    enumerate_enum_helper_t<enum_with_enum_list_1> _l_helper(
        get_enum_list<enum_with_enum_list_1>()
    );
    for (const auto& _l_data : read_data_from_file<test_data_t>("unit_test"))
    {
        const auto& [_l_enum1, _l_enum2, _l_less_than_res, _l_equal_res, _l_difference_res]{_l_data
        };
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(_l_helper.less_than(_l_enum1, _l_enum2) == _l_less_than_res)
            && _EXPR(_l_helper.equal(_l_enum1, _l_enum2) == _l_equal_res) &&
            _EXPR(_l_helper.difference(_l_enum1, _l_enum2) == _l_difference_res)
        );
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests enumerate_enum_helper_t::less_than, equal and "
                 "difference "
                 "throw correct errors when using unknown enums",
         .path = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        "Unit testing less_than, equal and difference throwing correct errors"
    );
    using test_data_t = std::tuple<
        std::underlying_type_t<enum_with_enum_list_1>,
        std::underlying_type_t<enum_with_enum_list_1>,
        u8string,
        u8string,
        u8string>;
    enumerate_enum_helper_t<enum_with_enum_list_1> _l_helper(
        get_enum_list<enum_with_enum_list_1>()
    );
    for (const auto& _l_data : read_data_from_file<test_data_t>("unit_test"))
    {
        const auto& [_l_enum1_as_underlying, _l_enum2_as_underlying, _l_error1, _l_error2, _l_error3]{
            _l_data
        };
        matcher_t _l_matcher;
        enum_with_enum_list_1 _l_enum1{ static_cast<enum_with_enum_list_1>(_l_enum1_as_underlying) };
        enum_with_enum_list_1 _l_enum2{ static_cast<enum_with_enum_list_1>(_l_enum2_as_underlying) };
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        bool _l_result = _l_helper.less_than(_l_enum1, _l_enum2);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error1
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        bool _l_result = _l_helper.equal(_l_enum1, _l_enum2);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error2
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        auto _l_result = _l_helper.difference(_l_enum1, _l_enum2);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error3
        );
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests enumerate_enum_helper_t::min and max",
         .path             = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    _BEGIN_MULTI_ELEMENT_BBA(_l_unit_tests, "Unit testing less_than and equal");
    using test_data_t
        = std::tuple<enum_with_enum_list_1, enum_with_enum_list_1, bool, bool>;
    enumerate_enum_helper_t<enum_with_enum_list_1> _l_helper(
        get_enum_list<enum_with_enum_list_1>()
    );
    _l_unit_tests
        += _BLOCK_CHECK(_EXPR(_l_helper.min() == enum_with_enum_list_1::one));
    _l_unit_tests
        += _BLOCK_CHECK(_EXPR(_l_helper.max() == enum_with_enum_list_1::three));
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests enumerate_enum_helper_t::increment, decrement "
                 "and print "
                 "throw correct errors when using unknown enum",
         .path = "abc_test_test::utility::enum",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    using namespace abc::utility::io;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        "Unit testing increment, decrement "
        "and print throwing correct errors"
    );
    using test_data_t = std::tuple<
        std::underlying_type_t<enum_with_enum_list_1>,
        u8string,
        u8string,
        u8string,
        u8string,
        u8string>;
    enumerate_enum_helper_t<enum_with_enum_list_1> _l_helper(
        get_enum_list<enum_with_enum_list_1>()
    );
    for (const auto& _l_data : read_data_from_file<test_data_t>("unit_test"))
    {
        const auto& [_l_enum_as_int, _l_error_increment, _l_error_increment_error_2, _l_error_decrement, _l_error_decrement_error_2, _l_error_print]{
            _l_data
        };
        matcher_t                                     _l_matcher;
        size_t                                        _l_size{0};
        enum_with_enum_list_1 _l_enum{static_cast<enum_with_enum_list_1>(_l_enum_as_int)
        };
        enum_with_enum_list_1 _l_min_enum{ _l_helper.min() };
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        auto _l_result = _l_helper.increment(_l_enum, _l_size, nullopt);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error_increment
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        auto _l_result = _l_helper.increment(_l_min_enum, _l_size, _l_enum);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error_increment_error_2
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        auto _l_result = _l_helper.decrement(_l_enum, _l_size, nullopt);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error_decrement
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        auto _l_result = _l_helper.decrement(_l_min_enum, _l_size, _l_enum);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error_decrement_error_2
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
        __BEGIN_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(_l_matcher);
        auto _l_result = _l_helper.print(_l_enum);
        __END_MAKE_MATCHER_TO_CHECK_EXCEPTION_TYPE_AND_MSG(
            _l_matcher, std::runtime_error, _l_error_print
        );
        _l_unit_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_unit_tests);
}