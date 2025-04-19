#pragma once

#include "abc_test/utility/rng/complete_global_seed.hpp"

_BEGIN_ABC_UTILITY_NS

class global_seed_t
{
public:
    __constexpr
    global_seed_t()
        = default;
    /*!
     * @brief Constructor which sets the object's inner_seed value to the
     * argument _a_seed.
     * @param _a_seed The object which sets this inner_seed.
     */
    __constexpr
    global_seed_t(const complete_global_seed_t& _a_seed) noexcept;
    /*!
     * @brief Sets the argument _a_seed using global_seed_t's internal state.
     *
     * If *this inner_seed() == nullopt, then _a_seed is set using the current
     * time of the system. Otherwise, _a_seed is set using the inner_seed's
     * value.
     *
     * @param _a_seed The complete_global_seed_t entity to set using this
     * object's data.
     */
    __constexpr void
        set_complete_seed(complete_global_seed_t& _a_seed) const noexcept;
    /*!
     * @brief Returns a cref to the objec'ts inner_seed.
     * @return A cref to this objects inner_seed value.
     */
    __constexpr const std::optional<complete_global_seed_t>&
                      inner_seed() const noexcept;
private:
    std::optional<complete_global_seed_t> _m_inner_seed;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<global_seed_t> : public printer_base_t<global_seed_t>
{
    __constexpr std::u8string
                run_printer(const global_seed_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_NS
__constexpr
global_seed_t::global_seed_t(
    const complete_global_seed_t& _a_seed
) noexcept
    : _m_inner_seed(_a_seed)
{}

__constexpr void
    global_seed_t::set_complete_seed(
        complete_global_seed_t& _a_seed
    ) const noexcept
{
    using namespace std;
    _a_seed = _m_inner_seed.value_or(
        complete_global_seed_t(static_cast<unsigned int>(time(0)))
    );
}

__constexpr const std::optional<complete_global_seed_t>&
                  global_seed_t::inner_seed() const noexcept
{
    return _m_inner_seed;
}

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

__constexpr std::u8string
            default_printer_t<global_seed_t>::run_printer(
        const global_seed_t& _a_object
    ) const
{
    using namespace std;
    using namespace abc::utility::printer;
    return default_printer_t<optional<complete_global_seed_t>>().run_printer(
        _a_object.inner_seed()
    );
}

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PARSER_NS

__constexpr result_t<global_seed_t>
            default_parser_t<global_seed_t>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    using namespace abc::utility::parser;
    using namespace abc::utility;
    return default_parser_t<optional<complete_global_seed_t>>()
        .run_parser(_a_parse_input)
        .transform(
            [](const optional<complete_global_seed_t>& _a_opt)
            {
                return _a_opt.has_value() ? global_seed_t(_a_opt.value())
                                          : global_seed_t();
            }
        );
}

_END_ABC_UTILITY_PARSER_NS