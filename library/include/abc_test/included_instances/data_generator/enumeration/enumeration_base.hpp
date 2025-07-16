#pragma once
#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <functional>
#include <variant>
#include "abc_test/external/bigint/BigUnsigned.hpp"

_BEGIN_ABC_DG_NS
#define _ENUMERATE_INDEX 0

#if _ENUMERATE_INDEX == 0
using enumerate_index_t = std::size_t;
#else
using enumerate_index_t = BigUnsigned;
#endif
using enumeration_diff_t = std::pair< enumerate_index_t, enumerate_index_t>;

__constexpr std::size_t to_size_t(const enumerate_index_t& _a_enumerate_index)
{
#if _ENUMERATE_INDEX == 0
    return _a_enumerate_index;
#else
    return _a_enumerate_index.toUnsignedLong();
#endif
}

template <typename T>
struct enumeration_base_t
{
public:
    using value_type_t = T;
    __constexpr virtual ~enumeration_base_t() = default;
    __constexpr_imp virtual bool
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept = 0;
    __constexpr_imp virtual bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept = 0;
    __constexpr virtual bool
        increment(
            T& _a_element,
            enumerate_index_t& _a_n_times_to_increment,
            const T& _a_min_value,
            const T& _a_max_value
        ) = 0;
    __constexpr virtual bool
        decrement(
            T& _a_element,
            enumerate_index_t& _a_n_times_to_increment,
            const T& _a_min_value,
            const T& _a_max_value
        ) = 0;
    __constexpr virtual enumeration_diff_t difference(
        const T& _a_arg1,
        const T& _a_arg2
    ) noexcept = 0;
};
template<typename T>
using enumeration_t = std::shared_ptr<enumeration_base_t<T>>;
_END_ABC_DG_NS

_BEGIN_ABC_NS
template<typename T>
__constexpr data_gen::enumeration_t<typename T::value_type_t> mk_enumeration(
    T&& _a_enumeration
) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template<typename T>
__constexpr_imp data_gen::enumeration_t<typename T::value_type_t> mk_enumeration(
    T&& _a_enumeration
) noexcept
{
    using namespace std;
    return make_shared<T>(_a_enumeration);
}
_END_ABC_NS