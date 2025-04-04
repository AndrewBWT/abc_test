#pragma once


#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr matcher_t
    contains(R&& _a_range, const T& _a_value) noexcept;

__no_constexpr matcher_t
    c_str_equal(const char* _a_str1, const std::string_view _a_str2) noexcept;

__no_constexpr_imp matcher_t
    expected_no_exception() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        true,
        fmt::format(u8"No exception thrown, which was the expected behaviour")
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_thrown(
        const std::exception& _a_exception
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        fmt::format(
            u8"An unexpected exception, of a type derived from {0} was thrown. "
            u8"The exception's what() function returned \"{1}\".",
            type_id<std::exception>(),
            unpack_string_to_u8string(_a_exception.what())
        )
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_of_unknown_type_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        fmt::format(u8"An unexpected exception of an unknown type was thrown.")
    ));
}

_END_ABC_NS

_BEGIN_ABC_NS

__no_constexpr_imp matcher_t
    c_str_equal(
        const char*            _a_str1,
        const std::string_view _a_str2
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    const bool _l_equal{std::strcmp(_a_str1, _a_str2.data()) == 0};
    return mk_matcher_using_result(matcher_result_t(
        _l_equal,
        fmt::format(
            u8"{0} {1} {2}",
            convert_string_to_u8string(_a_str1).value(),
            _l_equal ? u8"==" : u8"!=",
            convert_string_to_u8string(_a_str2).value()
        )
    ));
}

_END_ABC_NS