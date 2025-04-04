#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <fmt/base.h>
#include <string>
#include <vector>
#include <map>


_BEGIN_ABC_UTILITY_STR_NS
const char* const _c_quote_and_round_brackets{"()\""};
const char* const _c_comma{","};
const char* const _c_quote{"\""};

const char* const _c_l_square_bracket{"["};
const char* const _c_r_square_bracket{"]"};
const char* const _c_l_round_bracket("(");
const char* const _c_l_round_bracket_or_r_square_bracket("(]");
const char* const _c_r_round_bracket{")"};
const char* const _c_end_or_comma{",]"};
const char        _c_backslash = '\\';
/*!
 * @brief Checks hte current pos. If its set at npos, then the mode is set to
 * the max int. Else the mode is set to the argument.
 * @param _a_mode
 * @param _a_current_pos
 * @param _a_next_mode
 */
constexpr void
    process_mode(
        std::size_t&      _a_mode,
        const std::size_t _a_current_pos,
        const std::size_t _a_next_mode
    ) noexcept;
/*!
 * @brief Locates the relevant characters in the given string. Sets the position
 * reference to that index. If not foun,d the error message is set.
 * @param _a_str
 * @param _l_c_string_to_locate
 * @param _a_current_pos_ref
 * @param _a_error_string
 */
constexpr void
    locate_relevant_character(
        const std::u8string_view _a_str,
        const std::u8string_view            _l_c_string_to_locate,
        std::size_t&           _a_current_pos_ref,
        std::u8string&           _a_error_string
    ) noexcept;
__no_constexpr void
    locate_relevant_characters_and_change_mode(
        const std::u8string_view _a_str,
        std::size_t&           _a_current_pos_ref,
        const std::initializer_list<std::pair<char8_t, std::size_t>>&
                     _a_char_and_mode_list,
        std::u8string& _a_error_string,
        std::size_t& _a_mode_ref
    ) noexcept;
__no_constexpr void
skip_chars_and_strings_until_found(
    const std::u8string_view _a_str,
    std::size_t& _a_current_pos_ref,
    const std::initializer_list<std::pair<char8_t, char8_t>>& _a_ignore_mode,
    char8_t _a_increment_counter,
    char8_t _a_decrement_counter,
    const std::size_t _a_new_mode,
    const std::size_t _a_init_depth,
    std::u8string& _A_error_string,
    std::size_t& _a_mode_ref
) noexcept;
constexpr std::size_t
    maybe_set_data(
        const std::size_t _a_current_pos,
        const std::size_t _a_offset = 0
    ) noexcept;
/*!
 * @brief Processes string. Can change the mode if a closing quote is found.
 * Otherwise the mode isn't changed.
 * @param _a_str
 * @param _a_current_pos
 * @param _a_error_string
 * @param _a_mode
 * @param _a_finish_string_mode
 */
constexpr void
    process_string(
        const std::u8string_view _a_str,
        std::size_t&           _a_current_pos,
        std::u8string&           _a_error_string,
        std::size_t&           _a_mode,
        const std::size_t      _a_finish_string_mode
    ) noexcept;

constexpr void
    process_list_elements(
        const std::u8string_view    _a_str,
        std::size_t&              _a_current_pos,
        std::u8string&              _a_error_string,
        std::size_t&              _a_depth,
        std::vector<std::u8string>& _a_strs,
        const std::size_t         _a_start_pos,
        std::size_t&              _a_mode,
        const std::size_t         _a_mode_for_escape_string,
        const std::size_t         _a_mode_for_comma_or_end_list
    ) noexcept
{
    using namespace std;
    locate_relevant_character(
        _a_str, u8"()\"", _a_current_pos, _a_error_string
    );
    _a_depth = (_a_current_pos == string::npos) ? _a_depth
               : (_a_str[_a_current_pos] == _c_quote_and_round_brackets[1]
                  && _a_depth != 0)
                   ? _a_depth - 1
               : (_a_str[_a_current_pos] == _c_quote_and_round_brackets[0])
                   ? _a_depth + 1
                   : _a_depth;
    _a_mode  = (_a_current_pos == string::npos) ? numeric_limits<size_t>::max()
               : (_a_str[_a_current_pos] == _c_quote_and_round_brackets[1]
                 && _a_depth == 0)
                   ? _a_mode_for_comma_or_end_list
               : (_a_str[_a_current_pos] == _c_quote_and_round_brackets[2])
                   ? _a_mode_for_escape_string
                   : _a_mode;
    if (_a_depth == 0
        && _a_str[_a_current_pos] == _c_quote_and_round_brackets[1])
    {
        _a_strs.push_back(u8string(
            _a_str.substr(_a_start_pos, (_a_current_pos - _a_start_pos) + 1)
        ));
    }
}

constexpr void
    process_end_of_list(
        const std::u8string_view _a_str,
        std::size_t&           _a_current_pos,
        std::u8string&           _a_error_string,
        std::size_t&           _a_mode,
        const std::size_t      _a_mode_if_comma_found
    ) noexcept
{
    using namespace std;
    locate_relevant_character(
        _a_str, u8",]", _a_current_pos, _a_error_string
    );
    _a_mode = (_a_current_pos == string::npos
               || _a_str[_a_current_pos] == _c_end_or_comma[1])
                  ? numeric_limits<size_t>::max()
                  : _a_mode_if_comma_found;
}

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
constexpr void
    process_mode(
        std::size_t&      _a_mode,
        const std::size_t _a_current_pos,
        const std::size_t _a_next_mode
    ) noexcept
{
    using namespace std;
    _a_mode = (_a_current_pos == string::npos) ? numeric_limits<size_t>::max()
                                               : _a_next_mode;
}

constexpr void
    locate_relevant_character(
        const std::u8string_view _a_str,
        const std::u8string_view            _l_c_string_to_locate,
        std::size_t&           _a_current_pos_ref,
        std::u8string&           _a_error_string
    ) noexcept
{
    using namespace std;
    const size_t _l_old_current_pos{_a_current_pos_ref};
    _a_current_pos_ref
        = _a_str.find_first_of(_l_c_string_to_locate, _a_current_pos_ref);
    _a_error_string
        = (_a_current_pos_ref != u8string::npos)
              ? u8""
              : fmt::format(
                    u8"Expecting one of \"{0}\", but unable to find in string "
                    u8"\"{1}\". "
                    u8"Searching from position {2} (substring \"{3}\")",
                    _l_c_string_to_locate,
                    _a_str,
                    _l_old_current_pos,
                    _a_str.substr(_l_old_current_pos)
                );
}

__no_constexpr_imp void
    locate_relevant_characters_and_change_mode(
        const std::u8string_view _a_str,
        std::size_t&           _a_current_pos_ref,
        const std::initializer_list<std::pair<char8_t, std::size_t>>&
                     _a_char_and_mode_list,
        std::u8string& _a_error_string,
        std::size_t& _a_mode_ref
    ) noexcept
{
    using namespace std;
    const size_t _l_old_current_pos{_a_current_pos_ref};
    map<char, size_t> _l_map;
    u8string _l_strs_to_search;
    for (const pair<char8_t, size_t>& _a_char_and_mode : _a_char_and_mode_list)
    {
        _l_map.insert(_a_char_and_mode);
        _l_strs_to_search.push_back(_a_char_and_mode.first);
    }
    const size_t _l_new_pos
        = _a_str.find_first_of(_l_strs_to_search, _a_current_pos_ref);
    if (_l_new_pos != u8string::npos)
    {
        char8_t _l_char{ _a_str[_l_new_pos] };
        _a_mode_ref = _l_map.at(_l_char);
        _a_current_pos_ref = _l_new_pos;
    }
    else
    {
        _a_error_string = fmt::format(
            u8"Expecting one of \"{0}\", but unable to find in string "
            u8"\"{1}\". "
            u8"Searching from position {2} (substring \"{3}\")",
            _l_strs_to_search,
            _a_str,
            _l_old_current_pos,
            _a_str.substr(_l_old_current_pos)
        );
    }
}
__no_constexpr_imp void
skip_chars_and_strings_until_found(
    const std::u8string_view _a_str,
    std::size_t& _a_current_pos_ref,
    const std::initializer_list<std::pair<char8_t, char8_t>>& _a_ignore_mode,
    char8_t _a_increment_counter,
    char8_t _a_decrement_counter,
    const std::size_t _a_new_mode,
    const std::size_t _a_init_depth,
    std::u8string& _A_error_string,
    std::size_t& _a_mode_ref
) noexcept
{
    using namespace std;
    size_t _l_depth{ _a_init_depth };
    std::u8string _l_to_find = u8string(1, _a_increment_counter);
    _l_to_find.push_back(_a_decrement_counter);
    map<char8_t, char8_t> _l_map;
    for (const pair<char8_t, char8_t>& _l_pair_char : _a_ignore_mode)
    {
        _l_to_find.push_back(_l_pair_char.first);
        _l_map.insert(_l_pair_char);
    }
    optional<char8_t> _l_ignore_char;
    while (_l_depth > 0)
    {
        if (_l_ignore_char.has_value())
        {
            size_t _l_next_relevant_char = _a_str.find(_l_ignore_char.value(), _a_current_pos_ref);
            if (_l_next_relevant_char == string::npos)
            {
                _A_error_string = u8"coudln't find";
                return;
            }
            else
            {
                _a_current_pos_ref = _l_next_relevant_char+1;
                _l_ignore_char = optional<char>{};
            }
        }
        else
        {
            size_t _l_next_relevant_char = _a_str.find_first_of(_l_to_find, _a_current_pos_ref);
            if (_l_next_relevant_char == string::npos)
            {
                _A_error_string = u8"couldn't work";
                return;
            }
            char8_t _l_char{ _a_str[_l_next_relevant_char] };

            if (_l_char == _a_increment_counter)
            {
                _a_current_pos_ref = _l_next_relevant_char + 1;
                _l_depth++;
            }
            else if (_l_char == _a_decrement_counter)
            {
                _l_depth--;
                if (_l_depth > 0)
                {
                    _a_current_pos_ref = _l_next_relevant_char + 1;
                }
            }else
            {
                _a_current_pos_ref = _l_next_relevant_char + 1;
                _l_ignore_char = _l_map.at(_l_char);
            }
        }
    }
    _a_mode_ref = _a_new_mode;
}
constexpr std::size_t
    maybe_set_data(
        const std::size_t _a_current_pos,
        const std::size_t _a_offset
    ) noexcept
{
    using namespace std;
    return (_a_current_pos == string::npos) ? 0 : (_a_current_pos + _a_offset);
}

constexpr void
    process_string(
        const std::u8string_view _a_str,
        std::size_t&           _a_current_pos,
        std::u8string&           _a_error_string,
        std::size_t&           _a_mode,
        const std::size_t      _a_finish_string_mode
    ) noexcept
{
    using namespace std;
    locate_relevant_character(
        _a_str, u8"\"", _a_current_pos, _a_error_string
    );
    _a_mode = _a_current_pos == string::npos ? numeric_limits<size_t>::max()
              : (_a_str[_a_current_pos - 1] == _c_backslash)
                  ? _a_mode
                  : _a_finish_string_mode;
}

_END_ABC_UTILITY_STR_NS