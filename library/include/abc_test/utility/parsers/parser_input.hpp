#pragma once
#include "abc_test/utility/parsers/parser_exceptions.hpp"

_BEGIN_ABC_UTILITY_PARSER_NS

struct parser_input_t
{
public:
    __constexpr
    parser_input_t(const std::string_view _a_str) noexcept;
    __constexpr bool
        check_and_advance(const char _a_char_to_check_against) noexcept;
    __constexpr bool
        check_and_advance(const std::string_view _a_str_to_check_against
        ) noexcept;
    __constexpr void
        check_advance_and_throw(const char _a_char_to_check_against);
    __constexpr void
        check_advance_and_throw(const std::string_view _a_str_to_check_against);
    __no_constexpr void process_whitespace() noexcept;
    __constexpr void
                      advance(const std::size_t _a_new_itereator) noexcept;
    __constexpr const std::string_view
                      sv() const noexcept;
    __constexpr const std::string_view
                      sv(const std::size_t _a_size) const noexcept;
    __constexpr       std::size_t
                      size() const noexcept;
    __constexpr char
        operator*() const noexcept;
    __constexpr parser_input_t&
        operator++();
    __constexpr parser_input_t
        operator++(int);
    __constexpr bool
        at_end() const noexcept;
private:
    std::string_view                 _m_complete_string;
    std::size_t                      _m_elements_processed{0};
    std::string_view::const_iterator _m_cur_itt;
    std::string_view::const_iterator _m_end_itt;
};

__no_constexpr auto
    format_as(abc::utility::parser::parser_input_t _a_val);
_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PARSER_NS
__constexpr
parser_input_t::parser_input_t(
    const std::string_view _a_str
) noexcept
    : _m_cur_itt(_a_str.begin())
    , _m_end_itt(_a_str.end())
    , _m_complete_string(_a_str)
{}

__constexpr void
    parser_input_t::check_advance_and_throw(
        const char _a_char_to_check_against
    )
{
    using namespace std;
    if (not check_and_advance(_a_char_to_check_against))
    {
        throw parser_could_not_match_string_t(
            _m_complete_string,
            string_view(_m_cur_itt, _m_end_itt),
            string(1, _a_char_to_check_against)
        );
    }
}

__constexpr void
    parser_input_t::check_advance_and_throw(
        const std::string_view _a_str_to_check_against
    )
{
    using namespace std;
    if (not check_and_advance(_a_str_to_check_against))
    {
        throw parser_could_not_match_string_t(
            _m_complete_string,
            string_view(_m_cur_itt, _m_end_itt),
            _a_str_to_check_against
        );
    }
}
__no_constexpr_imp void parser_input_t::process_whitespace() noexcept
{
    while (std::isspace(*_m_cur_itt))
    {
        ++_m_cur_itt;
    }
}
__constexpr bool
    parser_input_t::check_and_advance(
        const char _a_char_to_check_against
    ) noexcept
{
    if (*_m_cur_itt == _a_char_to_check_against)
    {
        ++_m_cur_itt;
        return true;
    }
    else
    {
        return false;
    }
}

__constexpr bool
    parser_input_t::check_and_advance(
        const std::string_view _a_str_to_check_against
    ) noexcept
{
    using namespace std;
    const size_t _l_str_len{_a_str_to_check_against.size()};
    if ( _l_str_len <= std::distance(_m_cur_itt, _m_end_itt)
        && string_view(_m_cur_itt, _m_cur_itt + _l_str_len)
               == _a_str_to_check_against)
    {
        _m_cur_itt += _l_str_len;
        return true;
    }
    else
    {
        return false;
    }
}

__constexpr void
    parser_input_t::advance(
        const std::size_t _a_new_itereator
    ) noexcept
{
    _m_cur_itt += _a_new_itereator;
}

__constexpr const std::string_view
                  parser_input_t::sv() const noexcept
{
    return std::string_view(_m_cur_itt, _m_end_itt);
}

__constexpr const std::string_view
                  parser_input_t::sv(
        const std::size_t _a_size
    ) const noexcept
{
    return std::string_view(_m_cur_itt, _m_cur_itt + _a_size);
}

__constexpr std::size_t
            parser_input_t::size() const noexcept
{
    return std::distance(_m_cur_itt, _m_end_itt);
}

__constexpr char
    parser_input_t::operator*() const noexcept
{
    return *_m_cur_itt;
}

__constexpr parser_input_t&
    parser_input_t::operator++()
{
    if (_m_cur_itt == _m_end_itt)
    {
        throw parser_at_end_of_string_exception_t(
            _m_complete_string, _m_elements_processed
        );
    }
    else
    {
        ++_m_cur_itt;
    }
    return *this;
}

__constexpr parser_input_t
    parser_input_t::operator++(
        int
    )
{
    ++*this;
    return *this;
}

__constexpr bool
    parser_input_t::at_end() const noexcept
{
    return _m_cur_itt == _m_end_itt;
}

__no_constexpr_imp auto
    format_as(
        abc::utility::parser::parser_input_t _a_val
    )
{
    return _a_val.sv();
}

_END_ABC_UTILITY_PARSER_NS