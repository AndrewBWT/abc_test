#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/packing.hpp"

#include <source_location>
#include <stdexcept>
#include <fmt/xchar.h>

_BEGIN_ABC_NS

class unreachable_exception_t : public std::runtime_error
{
public:
    __constexpr
    unreachable_exception_t()
        = delete;

    __no_constexpr_imp
        unreachable_exception_t(
            const std::u8string_view _a_error
        ) noexcept
        : std::runtime_error(_ABC_NS_UTILITY_STR::pack_u8string_into_string(_a_error))
    {}
};

#if _TESTING_BUILD
constexpr bool unreachable_does_not_throw_exception = false;
#else
constexpr bool unreachable_does_not_throw_exception = true;
#endif


#if _TESTING_BUILD
    #define _ABC_UNREACHABLE(_a_str) throw abc::unreachable_exception_t(_a_str);
#else
    #define _UNREACHABLE(_a_str)           std::unreachable();
    #define _ABC_UNREACHABLE_ENUM(_a_enum) std::unreachable();
#endif

template <typename T>
requires std::is_enum_v<T>
__constexpr std::u8string
            unreachable_enum(
                const T                  _a_enum,
                const std::u8string_view _a_function_name
            )
        noexcept(
            unreachable_does_not_throw_exception
        )
{
#if _TESTING_BUILD
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    throw abc::unreachable_exception_t(fmt::format(
        u8"No code for enum of type "
        u8"\"{0}\" encountered in function "
        u8"\"{1}\". The enum's value has no known text "
        u8"representation. The enum, represented in its underlying type "
        u8"\"{2}\", has a value of {3}.",
        type_id<T>(),
        _a_function_name,
        type_id<underlying_type_t<T>>(),
        to_underlying(_a_enum)

    ));
#else
    std::unreachable();
#endif
}

template <typename T>
__constexpr std::u8string
            unreachable_variant(
                const std::u8string_view _a_function_name
            )
        noexcept(
            unreachable_does_not_throw_exception
        )
{
    using namespace _ABC_NS_UTILITY_STR;
#if _TESTING_BUILD
    using namespace std;
    throw abc::unreachable_exception_t(fmt::format(
        u8"No code for {0}. This may be due to a missed case, or a "
        u8"std::variant that is corrupted. Error occoured in function \"{0}\".",
        type_id<T>(),
        _a_function_name

    ));
#else
    std::unreachable();
#endif
}

_END_ABC_NS