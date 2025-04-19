#pragma once

#include "abc_test/utility/rng/complete_global_seed.hpp"

_BEGIN_ABC_UTILITY_NS

class global_seed_t
{
public:
    __constexpr
    global_seed_t()
        = default;

    __constexpr
    global_seed_t(const complete_global_seed_t& _a_seed) noexcept;
    __constexpr void
        set_complete_seed(complete_global_seed_t& _a_seed) const;
    __constexpr const
        std::optional<std::reference_wrapper<const complete_global_seed_t>>
        get_complete_global_seed_if_used() const noexcept;
    /*__constexpr bool
                is_not_set() const noexcept;
    __constexpr std::optional<complete_global_seed_t>
                complete_global_seed() const noexcept;*/
private:
    std::variant<std::monostate, complete_global_seed_t> _m_inner_seed
        = std::monostate{};
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

_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<global_seed_t> : public parser_base_t<global_seed_t>
{
    __constexpr result_t<global_seed_t>
                run_parser(parser_input_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PARSER_NS

/*!
 * formatter for registered_test_data_t
 */
template <>
struct fmt::formatter<abc::utility::global_seed_t> : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can't be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(abc::utility::global_seed_t _a_rtd, format_context& _a_ctx) const
        -> format_context::iterator;
};

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
    ) const
{
    using namespace std;
    if (auto _l_ptr{get_if<monostate>(&_m_inner_seed)}; _l_ptr != nullptr)
    {
        _a_seed
            = complete_global_seed_t(static_cast<unsigned int>(std::time(0)));
    }
    else if (auto _l_ptr{get_if<complete_global_seed_t>(&_m_inner_seed)};
             _l_ptr != nullptr)
    {
        _a_seed = *_l_ptr;
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_m_inner_seed);
    }
}

__constexpr const std::optional<std::reference_wrapper<const complete_global_seed_t>>
    global_seed_t::get_complete_global_seed_if_used() const noexcept
{
    using namespace std;
    if (const complete_global_seed_t
            * _l_ptr{get_if<complete_global_seed_t>(&_m_inner_seed)};
        _l_ptr != nullptr)
    {
        return reference_wrapper(*_l_ptr);
    }
    else
    {
        return nullopt;
    }
}

/*__constexpr bool
    global_seed_t::is_not_set() const noexcept
{
    using namespace std;
    return holds_alternative<monostate>(_m_inner_seed);
}

__constexpr std::optional<complete_global_seed_t>
            global_seed_t::complete_global_seed() const noexcept
{
    using namespace std;
    if (holds_alternative<complete_global_seed_t>(_m_inner_seed))
    {
        return get<complete_global_seed_t>(_m_inner_seed);
    }
    else
    {
        return nullopt;
    }
}*/

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

__constexpr std::u8string
            default_printer_t<global_seed_t>::run_printer(
        const global_seed_t& _a_parse_input
    ) const
{
    using namespace std;
    return u8"global_seed_t";
}

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PARSER_NS

__constexpr result_t<global_seed_t>
            default_parser_t<global_seed_t>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    const result_t<unsigned int> _l_variant_result{
        utility::parser::default_parser_t<unsigned int>().run_parser(
            _a_parse_input
        )
    };
    if (_l_variant_result.has_value())
    {
        return result_t<global_seed_t>(
            complete_global_seed_t(_l_variant_result.value())
        );
    }
    else
    {
        return unexpected(_l_variant_result.error());
    }
}

_END_ABC_UTILITY_PARSER_NS

__no_constexpr_imp auto
    fmt::formatter<abc::utility::global_seed_t>::format(
        abc::utility::global_seed_t _a_rtd,
        format_context&             _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        "}}",
        typeid(_a_rtd).name(),
        "_l_inner_seed",
        "placeholder"
        // _a_rtd._l_inner_seed
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}