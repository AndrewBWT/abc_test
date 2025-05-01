#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include <stdexcept>
#include "abc_test/utility/str/conversion.hpp"

_BEGIN_ABC_NS
class unreachable_exception_t : public std::runtime_error
{
public:
    __constexpr
        unreachable_exception_t()
        = delete;
    __no_constexpr_imp
        unreachable_exception_t(
            const std::u8string    _a_error
        ) noexcept
        : std::runtime_error(pack_u8string_into_string(_a_error))
    {

    }
};


#if _TESTING_BUILD
constexpr bool unreachable_does_not_throw_exception = false;
#else
constexpr bool unreachable_does_not_throw_exception = true;
#endif


#if _TESTING_BUILD
#define _ABC_UNREACHABLE(_a_str) throw abc::unreachable_exception_t(_a_str);
#else
#define _UNREACHABLE(_a_str) std::unreachable();
#endif
_END_ABC_NS