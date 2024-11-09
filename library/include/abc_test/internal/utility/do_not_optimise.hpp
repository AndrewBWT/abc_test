#pragma once
#include "abc_test/internal/ds/type_synonyms.hpp"
#include "abc_test/internal/global.hpp"

_BEGIN_ABC_NS
template <typename T>
__constexpr void
    do_not_optimise(
        const T& _a_value
    )
{
    using namespace _ABC_NS_UTILITY;
    using volatile_char_t = char const volatile;
    volatile_char_t* const _l_volatile_char{
        &reinterpret_cast<volatile_char_t&>(_a_value)
    };
    global::get_threads_volatie_ptr_ref()
        = reinterpret_cast<volatilte_void_ptr_t>(_l_volatile_char);
}

_END_ABC_NS