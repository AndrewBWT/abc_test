#pragma once

#include "abc_test/included_instances/data_generator/enumeration_schema.hpp"
#include "abc_test/internal/utility/limits/max_value_concept.hpp"
#include "abc_test/internal/utility/limits/min_value_concept.hpp"
// #include
// "abc_test/included_instances/data_generator/enumeration_data_object/max_value_concept.hpp"
// #include
// "abc_test/included_instances/data_generator/enumeration_data_object/min_value_concept.hpp"

#include <optional>

_BEGIN_ABC_DG_NS

template <typename T>
class enumeration_schema_from_m_to_n_t : public enumeration_schema_t<T>
{
public:
    __constexpr
    enumeration_schema_from_m_to_n_t(
        const T&                          _a_start_value,
        const T&                          _a_end_value,
        const std::optional<std::size_t>& _a_n_advancements
        = std::optional<std::size_t>()
    ) noexcept;
    __constexpr T virtual end_value(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept override;
    __constexpr bool virtual is_direction_forward(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept override;
    __constexpr std::size_t virtual n_advancements_per_advancement(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept override;
private:
    T                           _m_end_value;
    mutable std::optional<bool> _m_is_direction_forwards;
    std::optional<std::size_t>  _m_n_advancements_to_make;
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            all_values() noexcept;
template <typename T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            from_m_to_n(const T& _a_lower, const T& _a_upper) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            from_val_to_max(const T& _a_value) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            from_max_to_val(const T& _a_value) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            from_val_to_min(const T& _a_value) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            from_min_to_val(const T& _a_value) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            all_values_moving_forward() noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            all_values_moving_backward() noexcept;
template <typename T>
__constexpr std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
            from_m_to_n_using_k_values(
                const T&          _a_lower,
                const T&          _a_upper,
                const std::size_t _a_k
            ) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
    enumeration_schema_from_m_to_n_t<T>::enumeration_schema_from_m_to_n_t(
        const T&                          _a_start_value,
        const T&                          _a_end_value,
        const std::optional<std::size_t>& _a_n_advancements
    ) noexcept
    : enumeration_schema_t<T>(_a_start_value)
    , _m_end_value(_a_end_value)
    , _m_n_advancements_to_make(_a_n_advancements)
{}

template <typename T>
__constexpr_imp T
    enumeration_schema_from_m_to_n_t<T>::end_value(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
{
    return _m_end_value;
}

template <typename T>
__constexpr_imp bool
    enumeration_schema_from_m_to_n_t<T>::is_direction_forward(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
{
    if (not _m_is_direction_forwards.has_value())
    {
        _m_is_direction_forwards
            = _a_edo->less_than(this->_m_start_value, _m_end_value);
        ;
    }
    return _m_is_direction_forwards.value();
}

template <typename T>
__constexpr_imp std::size_t
    enumeration_schema_from_m_to_n_t<T>::n_advancements_per_advancement(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
{
    if (_m_n_advancements_to_make.has_value())
    {
        const size_t _l_n_advancements_to_make{ _m_n_advancements_to_make.value() };
        const auto [_l_divisor, _l_remainder]{
            _a_edo->difference(this->_m_start_value, _m_end_value)
        };
        const size_t _l_difference{ _l_divisor % _l_n_advancements_to_make };
        const size_t _l_divider{
            _m_n_advancements_to_make.value() - (_l_difference == 0 ? 1 : 2)
        };
        const size_t _l_base_advancements{_l_divisor / _l_divider};
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
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                all_values() noexcept
{
    return all_values_moving_forward<T>();
}

template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_to_n(
                    const T& _a_lower,
                    const T& _a_upper
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_shared<enumeration_schema_from_m_to_n_t<T>>(_a_lower, _a_upper);
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_val_to_max(
                    const T& _a_value
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(_a_value, max_value_t<T>().max_value());
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_max_to_val(
                    const T& _a_value
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(max_value_t<T>().max_value(), _a_value);
}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_val_to_min(
                    const T& _a_value
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(_a_value, min_value_t<T>().min_value());
}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_min_to_val(
                    const T& _a_value
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return from_m_to_n(min_value_t<T>().min_value(), _a_value);
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                all_values_moving_forward() noexcept
{
    using namespace _ABC_NS_DG;
    using namespace _ABC_NS_UTILITY;
    return from_m_to_n(
        min_value_t<T>().min_value(), max_value_t<T>().max_value()
    );
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                all_values_moving_backward() noexcept
{
    using namespace _ABC_NS_DG;
    using namespace _ABC_NS_UTILITY;
    return from_m_to_n(
        max_value_t<T>().max_value(), min_value_t<T>().min_value()
    );
}

template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_to_n_using_k_values(
                    const T&          _a_lower,
                    const T&          _a_upper,
                    const std::size_t _a_k
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_shared<enumeration_schema_from_m_to_n_t<T>>(
        _a_lower, _a_upper, optional<size_t>(_a_k)
    );
}

_END_ABC_NS