#pragma once

#include "abc_test/core/data_generator/data_generator_collection.hpp"
#include "abc_test/core/data_generator/data_generator_with_file_support.hpp"
#include "abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_to_n.hpp"

_BEGIN_ABC_DG_NS

template <typename T>
class enumeration_data_generator_t
{
public:
    using generator_type = T;
    using tertiary_type  = std::size_t;
    __constexpr
    enumeration_data_generator_t(const enumeration_schema_t<T>& _a_es) noexcept;
    __constexpr bool
        has_current_element() const noexcept;
    __constexpr bool
        generate_next();
    __constexpr const T&
        current_element() const noexcept;
    __constexpr const tertiary_type&
        tertiary_data() const noexcept;
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );
    __constexpr void
        reset() noexcept;
private:
    enumeration_schema_t<T> _m_enum_schema;
    generator_type          _m_current_element;
    tertiary_type           _m_tertiary_data;
    bool                    _m_has_current_element;
    T                       _m_start_value;
    T                       _m_end_value;
    bool                    _m_forward_direction;
    enumerate_index_t       _m_number_of_complete_advancements_to_end;
    enumerate_index_t       _m_remainder_after_all_advancements;
    enumerate_index_t       _m_n_advancements_per_generate_next;

    __constexpr bool
        next_element(const enumerate_index_t& _a_times_called) noexcept;
    __constexpr bool
        next_element_reference_arg(enumerate_index_t& _a_times_called) noexcept;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename... Args>
__constexpr _ABC_NS_DG::data_generator_collection_t<T>
            enumerate_data(
                const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
                Args... _a_file_reader_writers
            );

template <typename T, typename... Args>
__constexpr _ABC_NS_DG::data_generator_collection_t<T>
            enumerate_data(Args... _a_file_reader_writers);
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
    enumeration_data_generator_t<T>::enumeration_data_generator_t(
        const _ABC_NS_DG::enumeration_schema_t<T>& _a_es
    ) noexcept
    : _m_enum_schema(_a_es)
    , _m_current_element(_a_es->start_value())
    , _m_tertiary_data(tertiary_type{0})
    , _m_has_current_element{ true }
    , _m_start_value(_a_es->start_value())
    , _m_end_value(_a_es->end_value())
    , _m_forward_direction(_a_es->is_direction_forward())
    , _m_number_of_complete_advancements_to_end(
          _a_es->number_of_complete_advancements()
      )
    , _m_remainder_after_all_advancements(
          _a_es->remaining_entities_after_maximum_advancements()
      )
    , _m_n_advancements_per_generate_next(_a_es->n_advancements_per_advancement(
      ))
{}

template <typename T>
__constexpr_imp bool
    enumeration_data_generator_t<T>::has_current_element() const noexcept
{
    return _m_has_current_element;
}

template <typename T>
__constexpr_imp bool
    enumeration_data_generator_t<T>::generate_next()
{
    if (_m_has_current_element)
    {
        _m_tertiary_data++;
        _m_has_current_element
            = next_element(_m_n_advancements_per_generate_next);
    }
    return _m_has_current_element;
}

template <typename T>
__constexpr_imp const T&
    enumeration_data_generator_t<T>::current_element() const noexcept
{
    return _m_current_element;
}

template <typename T>
__constexpr_imp const enumeration_data_generator_t<T>::tertiary_type&
    enumeration_data_generator_t<T>::tertiary_data() const noexcept
{
    return _m_tertiary_data;
}

template <typename T>
__constexpr_imp void
    enumeration_data_generator_t<T>::set_generator_using_tertiary_data(
        const tertiary_type& _a_tertiary_data
    )
{
    auto _l_next_elemnet{ _a_tertiary_data - _m_tertiary_data };
    _m_tertiary_data       = _a_tertiary_data;
    _m_has_current_element = next_element(_l_next_elemnet);
}

template <typename T>
__constexpr_imp void
    enumeration_data_generator_t<T>::reset() noexcept
{
    _m_has_current_element = true;
    _m_current_element     = _m_start_value;
    _m_tertiary_data       = 0;
}

template <typename T>
__constexpr_imp bool
    enumeration_data_generator_t<T>::next_element(
        const enumerate_index_t& _a_times_called
    ) noexcept
{
    enumerate_index_t _l_times_called{_a_times_called};
    return next_element_reference_arg(_l_times_called);
}

template <typename T>
__constexpr_imp bool
    enumeration_data_generator_t<T>::next_element_reference_arg(
        enumerate_index_t& _a_times_called
    ) noexcept
{
    return _m_forward_direction
               ? _m_enum_schema->increment(
                     _m_current_element, _a_times_called, _m_start_value, _m_end_value
                 )
               : _m_enum_schema->decrement(
                     _m_current_element, _a_times_called, _m_start_value, _m_end_value
                 );
}

_END_ABC_DG_NS

_BEGIN_ABC_NS

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                enumerate_data(
                    const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
                    Args... _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_data_generator_with_file_support<
        enumeration_data_generator_t<T>>(
        enumeration_data_generator_t<T>(_a_es), _a_file_reader_writers...
    );
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                enumerate_data(
                    Args... _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return enumerate_data<T>(
        all_values<T>(), _a_file_reader_writers...
    );
}

_END_ABC_NS