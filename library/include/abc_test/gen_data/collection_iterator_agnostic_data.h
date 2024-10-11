#pragma once

#pragma once

#include "abc_test/core/ds/gen_data_memoization/gen_data_creation_data.h"
#include "abc_test/utility/internal/macros.h"


_BEGIN_ABC_NS

struct gen_data_collection_iterator_agnostic_data_t
{
public:
    __constexpr virtual ~gen_data_collection_iterator_agnostic_data_t(
    ) noexcept;
    __constexpr ds::gen_collection_creation_data_t
                get_repeat_test_repetition_data() noexcept;
    __constexpr ds::gen_collection_creation_data_t
        get_test_repetition_data() noexcept;
protected:
    __constexpr virtual ds::gen_data_creation_data_t
        generate_repeat_repetition_data() const noexcept
        = 0;
    __constexpr virtual ds::gen_data_creation_data_t
        generate_repetition_data() const noexcept
        = 0;
private:
    std::size_t _m_generation_collection_index;
    std::optional<ds::gen_data_creation_data_t> _m_flid;
    std::optional<ds::gen_data_creation_data_t> _m_repeat_flid;
};

_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp gen_data_collection_iterator_agnostic_data_t::
    ~gen_data_collection_iterator_agnostic_data_t() noexcept
{}

__constexpr_imp ds::gen_collection_creation_data_t
                gen_data_collection_iterator_agnostic_data_t::
        get_repeat_test_repetition_data() noexcept
{
    using namespace ds;
    if (not _m_repeat_flid.has_value())
    {
        _m_repeat_flid = generate_repeat_repetition_data();
    }
    return gen_collection_creation_data_t{
        _m_generation_collection_index, _m_repeat_flid.value()
    };
}

__constexpr_imp ds::gen_collection_creation_data_t
    gen_data_collection_iterator_agnostic_data_t::get_test_repetition_data(
    ) noexcept
{
    using namespace ds;
    if (not _m_flid.has_value())
    {
        _m_flid = generate_repetition_data();
    }
    return gen_collection_creation_data_t{
        _m_generation_collection_index, _m_flid.value()
    };
}

_END_ABC_NS