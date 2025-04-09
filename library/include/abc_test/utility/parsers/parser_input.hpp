#pragma once
#include "abc_test/utility/parsers/parser_exceptions.hpp"

_BEGIN_ABC_UTILITY_PARSER_NS

struct parser_input_t
{
public:
    __constexpr
    parser_input_t(const std::u8string_view _a_str) noexcept;
    __constexpr bool
        check_and_advance(const char32_t _a_char_to_check_against) noexcept;
    __constexpr bool
        check_and_advance(const std::u32string_view _a_str_to_check_against
        ) noexcept;
    __constexpr void
        check_advance_and_throw(const char32_t _a_char_to_check_against);
    __constexpr void
        check_advance_and_throw(
            const std::u32string_view _a_str_to_check_against
        );
    __no_constexpr void
                   process_whitespace() noexcept;
    __no_constexpr std::u32string
                   process_characters(const std::size_t _a_max_size) noexcept;
    __no_constexpr std::u32string
                   continue_until_char1_but_not_proceeded_by_char2(
                       const char32_t _a_char1,
                       const char32_t _a_char2
                   ) noexcept;
    __constexpr void
                         advance(const std::size_t _a_new_itereator) noexcept;
    __constexpr const    std::u8string_view
                         get_u8string() const noexcept;
    __constexpr const    std::u8string_view
                         get_u8string(const std::size_t _a_size) const noexcept;
    __no_constexpr const std::u32string
                         get_u32string() const noexcept;
    __no_constexpr const std::u32string
                      get_u32string(const std::size_t _a_size) const noexcept;
    __constexpr const result_t<std::string>
                      ascii_string() const noexcept;
    __no_constexpr const std::u32string
        take_string_containing(const std::u32string_view _a_str) noexcept;
    __constexpr std::size_t
                size() const noexcept;
    __no_constexpr char32_t
        operator*() const noexcept;
    __constexpr parser_input_t&
        operator++();
    __constexpr parser_input_t
        operator++(int);
    __constexpr bool
        at_end() const noexcept;
private:
    std::u8string_view                 _m_complete_string;
    std::size_t                        _m_elements_processed{0};
    std::u8string_view::const_iterator _m_cur_itt;
    std::u8string_view::const_iterator _m_end_itt;
};

__no_constexpr auto
    format_as(abc::utility::parser::parser_input_t _a_val);
_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PARSER_NS
__constexpr
parser_input_t::parser_input_t(
    const std::u8string_view _a_str
) noexcept
    : _m_cur_itt(_a_str.begin())
    , _m_end_itt(_a_str.end())
    , _m_complete_string(_a_str)
{}

__constexpr void
    parser_input_t::check_advance_and_throw(
        const char32_t _a_char_to_check_against
    )
{
    using namespace std;
    if (not check_and_advance(_a_char_to_check_against))
    {
        throw parser_could_not_match_string_t(
            _m_complete_string,
            u8string_view(_m_cur_itt, _m_end_itt),
            convert_u32string_to_u8string(
                std::u32string(1, _a_char_to_check_against)
            )
        );
    }
}

__constexpr void
    parser_input_t::check_advance_and_throw(
        const std::u32string_view _a_str_to_check_against
    )
{
    using namespace std;
    if (not check_and_advance(_a_str_to_check_against))
    {
        throw parser_could_not_match_string_t(
            _m_complete_string,
            u8string_view(_m_cur_itt, _m_end_itt),
            abc::convert_u32string_to_u8string(_a_str_to_check_against)
        );
    }
}

__no_constexpr_imp void
    parser_input_t::process_whitespace() noexcept
{
    while (std::isspace(*_m_cur_itt))
    {
        ++_m_cur_itt;
    }
}

__no_constexpr_imp std::u32string
                   parser_input_t::process_characters(
        const std::size_t _a_max_size
    ) noexcept
{
    using namespace std;
    u32string _l_str;
    size_t    _l_idx{0};
    while (_m_cur_itt < _m_end_itt && _l_idx < _a_max_size)
    {
        _l_str.push_back(utf8::next(_m_cur_itt, _m_end_itt));
        ++_l_idx;
    }
    return _l_str;
}

__no_constexpr_imp std::u32string
    parser_input_t::continue_until_char1_but_not_proceeded_by_char2(
        const char32_t _a_char1,
        const char32_t _a_char2
    ) noexcept
{
    using namespace std;
    u32string _l_str;
    size_t    _l_idx{0};
    while (_m_cur_itt < _m_end_itt)
    {
        char32_t _l_char{utf8::next(_m_cur_itt, _m_end_itt)};
        if (_l_char == _a_char1)
        {
            if (_l_idx > 0 && _l_str.back() == _a_char2)
            {
                // Replace the last character, as it is currently the escape sequence
                _l_str.back() = _l_char;
            }
            else
            {
                break;
            }
        }
        else
        {
            _l_str.push_back(_l_char);
        }
        ++_l_idx;
    }
    return _l_str;
}

__constexpr bool
    parser_input_t::check_and_advance(
        const char32_t _a_char_to_check_against
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
        const std::u32string_view _a_str_to_check_against
    ) noexcept
{
    using namespace std;
    u32string_view::const_iterator _l_itt{_a_str_to_check_against.begin()};
    const u32string_view::const_iterator _l_end{_a_str_to_check_against.end()};
    u8string_view::const_iterator        _l_curr_itt_cpy{_m_cur_itt};
    while (_l_itt < _l_end)
    {
        if (_m_cur_itt < _m_end_itt)
        {
            char32_t _l_char{utf8::next(_l_curr_itt_cpy, _m_end_itt)};
            if (_l_char != *_l_itt)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        ++_l_itt;
    }
    _m_cur_itt = _l_curr_itt_cpy;
    return true;
}

__constexpr void
    parser_input_t::advance(
        const std::size_t _a_new_itereator
    ) noexcept
{
    size_t _l_idx{0};
    while (_m_cur_itt < _m_end_itt && _l_idx < _a_new_itereator)
    {
        utf8::next(_m_cur_itt, _m_end_itt);
        ++_l_idx;
    }
}

__constexpr const std::u8string_view
                  parser_input_t::get_u8string() const noexcept
{
    using namespace std;
    return u8string_view(_m_cur_itt, _m_end_itt);
}

__constexpr const std::u8string_view
                  parser_input_t::get_u8string(
        const std::size_t _a_size
    ) const noexcept
{
    using namespace std;
    return u8string_view(_m_cur_itt, _m_cur_itt + _a_size);
}

__no_constexpr_imp const std::u32string
                         parser_input_t::get_u32string() const noexcept
{
    using namespace std;
    return convert_u8string_to_u32string(u8string_view(_m_cur_itt, _m_end_itt));
}

__no_constexpr_imp const std::u32string
                         parser_input_t::get_u32string(
        const std::size_t _a_size
    ) const noexcept
{
    using namespace std;
    return convert_u8string_to_u32string(
        u8string_view(_m_cur_itt, _m_cur_itt + _a_size)
    );
}

__constexpr const result_t<std::string>
                  parser_input_t::ascii_string() const noexcept
{
    using namespace std;
    return convert_u8string_to_string(u8string_view(_m_cur_itt, _m_end_itt));
}

__no_constexpr_imp const std::u32string
                     parser_input_t::take_string_containing(
        const std::u32string_view _a_str
    ) noexcept
{
    using namespace std;
    u32string _l_rv;

    while (_m_cur_itt < _m_end_itt)
    {
        char32_t _l_char{ utf8::peek_next(_m_cur_itt, _m_end_itt) };
        if (_a_str.contains(_l_char))
        {
            _l_rv.push_back(_l_char);
            utf8::next(_m_cur_itt, _m_end_itt);
        }
        else
        {
            break;
        }
    }
    return _l_rv;
}

__constexpr std::size_t
            parser_input_t::size() const noexcept
{
    return std::distance(_m_cur_itt, _m_end_itt);
}

__no_constexpr_imp char32_t
    parser_input_t::operator*() const noexcept
{
    return utf8::peek_next(_m_cur_itt, _m_end_itt);
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
    return _a_val.get_u8string();
}

_END_ABC_UTILITY_PARSER_NS