#pragma once

#include "abc_test/included_instances/data_generator/enumeration/enumeration_data_object.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema_base.hpp"
#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"
#include "concepts"

_BEGIN_ABC_DG_NS

template <typename T>
struct enumeration_based_random_generator_object_t
    : public random_generator_base_t<T>
{
public:
    __constexpr
    enumeration_based_random_generator_object_t(
        const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
        const _ABC_NS_DG::enumeration_t<T>&        _a_edo
    ) noexcept;
    __no_constexpr virtual T
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    enumeration_t<T> _m_enum_object;
    T                _m_start_value;
    T                _m_end_value;
    bool             _m_forward_direction;
    std::size_t      _m_number_of_complete_advancements_to_end;
    std::size_t      _m_remainder_after_all_advancements;
    std::size_t      _m_n_advancements_per_generate_next;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr _ABC_NS_DG::random_generator_t<T>
            using_enumeration_generator(
                const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
                const _ABC_NS_DG::enumeration_t<T >&
                    _a_enumerate_base
            ) noexcept;

template <typename T>
__constexpr _ABC_NS_DG::random_generator_t<T>
    using_enumeration_generator(const _ABC_NS_DG::enumeration_schema_t<T>& _a_es
    ) noexcept;
template <typename T>
__constexpr _ABC_NS_DG::random_generator_t<T>
            using_enumeration_generator(
                const _ABC_NS_DG::enumeration_schema_t<T>& _a_enumerate_base
            ) noexcept;
template <typename T>
__constexpr _ABC_NS_DG::random_generator_t<T>
            using_enumeration_generator() noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS template <typename T>
__constexpr_imp
    enumeration_based_random_generator_object_t<T>::
        enumeration_based_random_generator_object_t(
            const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
            const _ABC_NS_DG::enumeration_t<T>&        _a_edo
        ) noexcept
    : _m_enum_object(_a_edo)
    , _m_start_value(_a_es->start_value())
    , _m_end_value(_a_es->end_value(_a_edo))
    , _m_forward_direction(_a_es->is_direction_forward(_a_edo))
    , _m_number_of_complete_advancements_to_end(
          _a_es->number_of_complete_advancements(_a_edo)
      )
    , _m_remainder_after_all_advancements(
          _a_es->remaining_entities_after_maximum_advancements(_a_edo)
      )
    , _m_n_advancements_per_generate_next(
          _a_es->n_advancements_per_advancement(_a_edo)
      )
{}

template <typename T>
__no_constexpr T
    enumeration_based_random_generator_object_t<T>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    size_t _l_times_called{
        _a_rnd_generator() % _m_number_of_complete_advancements_to_end
    };
    T _l_rv{_m_start_value};
    _m_forward_direction
        ? _m_enum_object->increment(_l_rv, _l_times_called, _m_end_value)
        : _m_enum_object->decrement(_l_rv, _l_times_called, _m_end_value);
    return _l_rv;
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp _ABC_NS_DG::random_generator_t<T>
                using_enumeration_generator(
                    const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
                    const _ABC_NS_DG::enumeration_t<T>&
                        _a_enumerate_base
                ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return make_shared<enumeration_based_random_generator_object_t<T>>(
        _a_es, _a_enumerate_base
    );
}

template <typename T>
__constexpr_imp _ABC_NS_DG::random_generator_t<T>
                using_enumeration_generator(
                    const _ABC_NS_DG::enumeration_schema_t<T>& _a_es
                ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return using_enumeration_generator(
        _a_es, default_enumeration<T>()
    );
}

template <typename T>
__constexpr_imp _ABC_NS_DG::random_generator_t<T>
                using_enumeration_generator(
                    const _ABC_NS_DG::enumeration_t<T>&
                        _a_enumerate_base
                ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return using_enumeration_generator(all_values<T>(), _a_enumerate_base);
}

template <typename T>
__constexpr_imp _ABC_NS_DG::random_generator_t<T>
                using_enumeration_generator() noexcept
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return using_enumeration_generator(all_values<T>());
}

_END_ABC_NS