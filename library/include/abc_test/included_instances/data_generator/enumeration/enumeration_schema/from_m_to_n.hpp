#pragma once

#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema_base.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
// #include
// "abc_test/included_instances/data_generator/enumeration_data_object/max_value_concept.hpp"
// #include
// "abc_test/included_instances/data_generator/enumeration_data_object/min_value_concept.hpp"

#include <optional>

_BEGIN_ABC_DG_NS

template <typename T>
class enumeration_schema_from_m_to_n_t : public enumeration_schema_base_t<T>
{
public:
    __constexpr
    enumeration_schema_from_m_to_n_t(
        const T&                          _a_start_value,
        const T&                          _a_end_value,
        const std::optional<std::size_t>& _a_n_advancements = std::nullopt,
        const enumeration_t<T>& _a_enumeration = default_enumeration<T>()
    ) noexcept;
    __constexpr
    enumeration_schema_from_m_to_n_t(
        const T&                _a_start_value,
        const T&                _a_end_value,
        const enumeration_t<T>& _a_enumeration
    ) noexcept;
    __constexpr T virtual end_value() const noexcept override;
    __constexpr bool virtual is_direction_forward() const noexcept override;
    __no_constexpr enumerate_index_t virtual n_advancements_per_advancement(
    ) const noexcept override;
private:
    T                                _m_end_value;
    mutable std::optional<bool>      _m_is_direction_forwards;
    std::optional<enumerate_index_t> _m_n_advancements_to_make;
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            all_values(
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;

template <typename T, typename R = std::initializer_list<T>>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            enumerate_list(
                const R&& _a_elements
            ) noexcept
{
    return all_values_moving_forward<T>(mk_enumeration_list<T>(_a_elements));
}

template <typename T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_m_to_n(
                const T&                            _a_lower,
                const T&                            _a_upper,
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_val_to_max(
                const T&                            _a_value,
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_max_to_val(
                const T&                      _a_valueconst,
                _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_val_to_min(
                const T&                            _a_value,
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_min_to_val(
                const T&                            _a_value,
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            all_values_moving_forward(
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            all_values_moving_backward(
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
template <typename T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_m_to_n_using_k_values(
                const T&                            _a_lower,
                const T&                            _a_upper,
                const std::size_t                   _a_k,
                const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                = default_enumeration<T>()
            ) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr
enumeration_schema_from_m_to_n_t<T>::enumeration_schema_from_m_to_n_t(
    const T&                          _a_start_value,
    const T&                          _a_end_value,
    const std::optional<std::size_t>& _a_n_advancements,
    const enumeration_t<T>&           _a_enumeration
) noexcept
    : enumeration_schema_base_t<T>(_a_start_value, _a_enumeration)
    , _m_end_value(_a_end_value)
    , _m_n_advancements_to_make(_a_n_advancements)
{}

template <typename T>
__constexpr
enumeration_schema_from_m_to_n_t<T>::enumeration_schema_from_m_to_n_t(
    const T&                _a_start_value,
    const T&                _a_end_value,
    const enumeration_t<T>& _a_enumeration
) noexcept
    : enumeration_schema_from_m_to_n_t<T>(
          _a_start_value,
          _a_end_value,
          std::nullopt,
          _a_enumeration
      )
{}

template <typename T>
__constexpr_imp T
    enumeration_schema_from_m_to_n_t<T>::end_value() const noexcept
{
    return _m_end_value;
}

template <typename T>
__constexpr_imp bool
    enumeration_schema_from_m_to_n_t<T>::is_direction_forward() const noexcept
{
    if (not _m_is_direction_forwards.has_value())
    {
        _m_is_direction_forwards = this->_m_enumeration->less_than(
            this->_m_start_value, _m_end_value
        );
        ;
    }
    return _m_is_direction_forwards.value();
}

template <typename T>
__no_constexpr_imp enumerate_index_t
    enumeration_schema_from_m_to_n_t<T>::n_advancements_per_advancement(
    ) const noexcept
{
    if (_m_n_advancements_to_make.has_value())
    {
        const enumerate_index_t _l_n_advancements_to_make{
            _m_n_advancements_to_make.value()
        };
        const auto [_l_divisor, _l_remainder]{
            this->_m_enumeration->difference(this->_m_start_value, _m_end_value)
        };
        const enumerate_index_t _l_difference{
            _l_divisor % _l_n_advancements_to_make
        };
        const enumerate_index_t _l_divider{
            _l_n_advancements_to_make - (_l_difference == 0 ? 1 : 2)
        };
        const enumerate_index_t _l_base_advancements{_l_divisor / _l_divider};
        return _l_base_advancements;
    }
    else
    {
        return 1;
    }
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                all_values(
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    return all_values_moving_forward<T>(_a_enumerate_base);
}

template <typename T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_m_to_n(
                    const T&                            _a_lower,
                    const T&                            _a_upper,
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_shared<enumeration_schema_from_m_to_n_t<T>>(
        _a_lower, _a_upper, _a_enumerate_base
    );
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_val_to_max(
                    const T&                            _a_value,
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(
        _a_value, max_value_t<T>().max_value(), _a_enumerate_base
    );
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_max_to_val(
                    const T&                            _a_value,
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(
        max_value_t<T>().max_value(), _a_value, _a_enumerate_base
    );
}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_val_to_min(
                    const T&                            _a_value,
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(
        _a_value, min_value_t<T>().min_value(), _a_enumerate_base
    );
}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_min_to_val(
                    const T&                            _a_value,
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace _ABC_NS_UTILITY;
    return from_m_to_n(
        min_value_t<T>().min_value(), _a_value, _a_enumerate_base
    );
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                all_values_moving_forward(
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace _ABC_NS_UTILITY;
    return from_m_to_n(
        min_value_t<T>().min_value(),
        max_value_t<T>().max_value(),
        _a_enumerate_base
    );
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                all_values_moving_backward(
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace _ABC_NS_UTILITY;
    return from_m_to_n(
        max_value_t<T>().max_value(),
        min_value_t<T>().min_value(),
        _a_enumerate_base
    );
}

template <typename T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_m_to_n_using_k_values(
                    const T&                            _a_lower,
                    const T&                            _a_upper,
                    const std::size_t                   _a_k,
                    const _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_shared<enumeration_schema_from_m_to_n_t<T>>(
        _a_lower, _a_upper, optional<size_t>(_a_k), _a_enumerate_base
    );
}

_END_ABC_NS