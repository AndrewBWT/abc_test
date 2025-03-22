#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
_BEGIN_ABC_UTILITY_NS
using rng_counter_t = std::size_t;
_END_ABC_UTILITY_NS

/*class rng_counter_t
{
public:
    __constexpr
    rng_counter_t() noexcept;
    __constexpr explicit rng_counter_t(const std::size_t _a_start) noexcept;
    __constexpr std::size_t
                operator()() const noexcept;
    __constexpr void
        increment() noexcept;
    __constexpr bool
        operator<(const rng_counter_t& _a_counter) const noexcept;
    __constexpr rng_counter_t
        offset(const std::size_t _a_offset) const noexcept;
private:
    std::size_t _m_index;
};

_END_ABC_UTILITY_NS
_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<rng_counter_t> : public parser_base_t<rng_counter_t>
{
    __constexpr result_t<rng_counter_t>
                run_parser(parser_input_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<rng_counter_t> : public printer_base_t<rng_counter_t>
{
    __no_constexpr_imp std::string
                       run_printer(const rng_counter_t& _a_element) const;
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_NS
__constexpr
rng_counter_t::rng_counter_t() noexcept
    : rng_counter_t(0)
{}

__constexpr
rng_counter_t::rng_counter_t(
    const std::size_t _a_start
) noexcept
    : _m_index(_a_start)
{}

__constexpr std::size_t
            rng_counter_t::operator()() const noexcept
{
    return _m_index;
}

__constexpr void
    rng_counter_t::increment() noexcept
{
    _m_index++;
}

__constexpr bool
    rng_counter_t::operator<(
        const rng_counter_t& _a_counter
    ) const noexcept
{
    return this->_m_index < _a_counter._m_index;
}

__constexpr rng_counter_t
    rng_counter_t::offset(
        const std::size_t _a_offset
    ) const noexcept
{
    return rng_counter_t(_m_index + _a_offset);
}

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PARSER_NS

__constexpr result_t<rng_counter_t>
            default_parser_t<rng_counter_t>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    const result_t<rng_counter_t> _l_result{
        utility::parser::default_parser_t<std::size_t>().run_parser(
            _a_parse_input
        )
    };
    if (_l_result.has_value())
    {
        return result_t<rng_counter_t>(rng_counter_t(_l_result.value()));
    }
    else
    {
        return unexpected(_l_result.error());
    }
}

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

__no_constexpr_imp std::string
                   default_printer_t<rng_counter_t>::run_printer(
        const rng_counter_t& _a_element
    ) const
{
    using namespace std;
    return to_string(_a_element());
}

_END_ABC_UTILITY_PRINTER_NS*/