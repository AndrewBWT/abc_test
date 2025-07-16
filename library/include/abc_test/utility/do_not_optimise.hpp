#pragma once
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/core/global.hpp"

_BEGIN_ABC_NS

/*!
 * @brief Ensures that a value can be created and is not optimised away.
 *
 * This code is taken from Google's benchmark library (which can be found here:
 * https://github.com/google/benchmark/tree/main
 * 
 * The code can be found in:
 * 
 * benchmark/include/benchmark/benchmark.h
 *
 * Orignally the code was this:
 *
 * template <class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp const& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

We have changed it slightly for our needs.
 */
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