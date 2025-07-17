#pragma once

#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test/utility/types.hpp"

#include <charconv>
#include <fmt/base.h>
#include <fmt/xchar.h>
#include <source_location>
#include <string>
#include <vector>

_BEGIN_ABC_UTILITY_STR_NS
/*!
 * Splits a string using the given delimiter
 */
template <typename T>
__constexpr std::vector<std::basic_string<T>>
            split_string(
                const std::basic_string_view<T> _a_str,
                const std::basic_string_view<T> _a_delimiter
            ) noexcept;
__constexpr std::vector<std::string_view>
            split_string_sv(
                const std::string_view _a_str,
                const std::string_view delimiter
            ) noexcept;
__no_constexpr std::string
               location_string(const std::source_location& _a_sl) noexcept;
__constexpr    std::string
               quote(const std::string_view _a_str) noexcept;
__constexpr    std::string
    indent(const std::size_t _a_size, const std::string_view _a_str) noexcept;
__constexpr const std::u8string_view
                  return_str_if_next_index_in_bound(
                      const std::size_t        _a_index_to_check,
                      const std::size_t        _a_max_index,
                      const std::u8string_view _a_str
                  ) noexcept;
__constexpr    std::u8string
               to_hex(const std::u8string_view _a_str) noexcept;
__constexpr    result_t<std::u8string>
               from_hex(const std::u8string_view _a_str);
__constexpr    std::u8string
               from_hex_with_exception(const std::u8string_view _a_str);
__no_constexpr std::string
               remove_whitespace(const std::string_view _a_str) noexcept;

template <typename T>
__constexpr std::basic_string<T>
            make_focused_string(
                const std::basic_string_view<T> _a_sv,
                const std::size_t               _a_idx,
                const std::size_t               _a_limit = 10
            ) noexcept
{
    using namespace std;
    auto will_overflow_unsigned = [&](size_t a, size_t b) -> size_t {
        return b > std::numeric_limits<size_t>::max() - a;
        };
    if (_a_sv.empty())
    {
        return basic_string<T>();
    }
    else
    {
        const size_t _l_idx{_a_idx >= _a_sv.size() ? _a_sv.size() - 1 : _a_idx};
        const size_t _l_before_offset{_a_limit};
        const size_t _l_after_offset{_a_limit + (_a_limit == numeric_limits<size_t>::max() ? 0 : 1)};
        const bool _l_enough_elements_before{_l_idx > _l_before_offset};
        bool _l_enough_elements_after;
        if (will_overflow_unsigned(_l_idx, _l_after_offset))
        {
            _l_enough_elements_after = false;
        }
        else
        {
            _l_enough_elements_after = _l_idx + _l_after_offset < _a_sv.size();
        }
        const size_t _l_after_offset_pos{_l_idx + _l_after_offset};
        const size_t _l_before_offset_pos{_l_idx - _l_before_offset};
        const size_t _l_length{_l_after_offset_pos - _l_before_offset_pos};
        const basic_string_view<T> _l_sv{
            _l_enough_elements_before
                ? (_l_enough_elements_after
                       ? _a_sv.substr(_l_before_offset_pos, _l_length)
                       : _a_sv.substr(_l_before_offset_pos))
                : (_l_enough_elements_after
                       ? _a_sv.substr(0, _l_after_offset_pos)
                       : _a_sv)
        };
        basic_string<T> _l_dots{};
        for (size_t _l_idx{0}; _l_idx < 3; ++_l_idx)
        {
            _l_dots.push_back(static_cast<T>(0x2E));
        }
        basic_string<T> _l_rv;
        if (_l_enough_elements_before)
        {
            _l_rv.append(_l_dots);
        }
        _l_rv.append(_l_sv);
        if (_l_enough_elements_after)
        {
            _l_rv.append(_l_dots);
        }
        return _l_rv;
    }
}

template <typename T>
__constexpr std::basic_string<T>
            make_focused_string(
                const std::basic_string<T>& _a_sv,
                const std::size_t           _a_idx,
                const std::size_t           _a_limit = 10
            ) noexcept
{
    using namespace std;
    return make_focused_string(basic_string_view<T>(_a_sv), _a_idx, _a_limit);
}

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS

template <typename T>
__constexpr_imp std::vector<std::basic_string<T>>
                split_string(
                    const std::basic_string_view<T> _a_str,
                    const std::basic_string_view<T> _a_delimiter
                ) noexcept
{
    using namespace std;
    if (_a_str.empty())
    {
        return {};
    }
    size_t          pos_start = 0, pos_end, delim_len = _a_delimiter.size();
    basic_string<T> token;
    vector<basic_string<T>> res;

    while ((pos_end = _a_str.find(_a_delimiter, pos_start))
           != basic_string<T>::npos)
    {
        token     = _a_str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(basic_string<T>(_a_str.substr(pos_start)));
    return res;
}

__constexpr_imp std::vector<std::string_view>
                split_string_sv(
                    const std::string_view _a_str,
                    const std::string_view _a_delimiter
                ) noexcept
{
    using namespace std;
    if (_a_str == "")
    {
        return {};
    }
    size_t              pos_start = 0, pos_end, delim_len = _a_delimiter.size();
    string_view         token;
    vector<string_view> res;

    while ((pos_end = _a_str.find(_a_delimiter, pos_start)) != string::npos)
    {
        token     = _a_str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(_a_str.substr(pos_start));
    return res;
}

__no_constexpr_imp std::string
                   location_string(
                       const std::source_location& _a_sl
                   ) noexcept
{
    using namespace std;
    // A formalised way of showing a location
    return string{fmt::format("{0}:{1}", _a_sl.file_name(), _a_sl.line())};
}

__constexpr_imp std::string
                quote(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    return string("\"").append(_a_str).append("\"");
}

__constexpr_imp std::string
                indent(
                    const std::size_t      _a_size,
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    return string(_a_size, ' ').append(_a_str);
}

__constexpr_imp const std::u8string_view
                      return_str_if_next_index_in_bound(
                          const std::size_t        _a_index_to_check,
                          const std::size_t        _a_max_index,
                          const std::u8string_view _a_str
                      ) noexcept
{
    return _a_index_to_check + 1 < _a_max_index ? _a_str : u8"";
}

__constexpr_imp std::u8string
                to_hex(
                    const std::u8string_view _a_str
                ) noexcept
{
    using namespace std;
    vector<unsigned char> _l_chars;
    for (char _l_char : _a_str)
    {
        _l_chars.push_back(static_cast<unsigned char>(_l_char));
    }
    u8string _l_rv;
    for (unsigned char _l_char : _l_chars)
    {
        _l_rv.append(fmt::format(u8"{:x}", _l_char));
    }
    return _l_rv;
}

__constexpr_imp result_t<std::u8string>
                from_hex(
                    const std::u8string_view _a_str
                )
{
    using namespace utility::str;
    using namespace std;
    if ((_a_str.size() % 2) != 0)
    {
        return unexpected(
            fmt::format(u8"Compressed repetition_tree_t is in the form a "
                        u8"string in hex. As "
                        u8"each hex digit requires two bytes, the total size "
                        u8"of the string "
                        u8"must be divisble by 2. This string is not")
        );
    }
    else
    {
        const result_t<string> _l_str_result{convert_unicode_to_ascii(_a_str)};
        if (_l_str_result.has_value())
        {
            // Parse the bytes.
            const string_view _l_str{_l_str_result.value()};
            const char*       _l_position = _l_str.data();
            vector<char8_t>   _l_chars;
            for (size_t _l_idx{0}; _l_idx < (_l_str.size() / 2); ++_l_idx)
            {
                unsigned char value{ 0 };
                string _l_str_to_process(_l_position, _l_position + 2);
                auto [_l_ptr, _l_ec]
                    = from_chars(_l_position, _l_position + 2, value, 16);
                _l_position += 2;
                // If no error, pass the byte to the list of chars
                if (_l_ec == errc())
                {
                    _l_chars.push_back(char8_t(value));
                }
                else
                {
                    // Else return failure
                    return unexpected{fmt::format(
                        u8"Could not parse byte pair {0}{1}",
                        *_l_position,
                        *(_l_position + 1)
                    )};
                }
            }
            return u8string(_l_chars.begin(), _l_chars.end());
        }
        else
        {
            return unexpected(fmt::format(
                u8"Could not convert std::u8string to std::string. Error was "
                u8"\"{0}\"",
                _l_str_result.error()
            ));
        }
    }
}

__constexpr_imp std::u8string
                from_hex_with_exception(
                    const std::u8string_view _a_str
                )
{
    using namespace std;
    result_t<u8string> _l_res{from_hex(_a_str)};
    if (not _l_res.has_value())
    {
        throw errors::test_library_exception_t(fmt::format(
            u8"Could not parse \"{0}\" from hex. Reason given is \"{1}\".",
            _a_str,
            _l_res.error()
        ));
    }
    else
    {
        return _l_res.value();
    }
}

__no_constexpr_imp std::u8string
                   remove_whitespace(
                       const std::u8string_view _a_str
                   ) noexcept
{
    using namespace std;
    u8string _l_rv{_a_str};
    _l_rv.erase(
        std::remove_if(
            _l_rv.begin(),
            _l_rv.end(),
            [](char c)
            {
                return std::isspace(static_cast<unsigned char>(c));
            }
        ),
        _l_rv.end()
    );
    return _l_rv;
}

_END_ABC_UTILITY_STR_NS