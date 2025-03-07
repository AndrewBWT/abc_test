#pragma once

#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <fmt/base.h>
#include <string>
#include <vector>
#include <source_location>
#include <charconv>

#include "abc_test/utility/types.hpp"

_BEGIN_ABC_UTILITY_STR_NS
/*!
 * Appends together a set of strings into a single output string.
 */
__constexpr std::string
    create_string(const std::vector<std::string_view>& _a_strs) noexcept;
/*!
 * Splits a string using the given delimiter
 */
__constexpr std::vector<std::string>
            split_string(
                const std::string_view _a_str,
                const std::string_view delimiter
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
__constexpr const std::string_view
                  return_str_if_next_index_in_bound(
                      const std::size_t      _a_index_to_check,
                      const std::size_t      _a_max_index,
                      const std::string_view _a_str
                  ) noexcept;
__constexpr std::string
            to_hex(const std::string_view _a_str) noexcept;
__constexpr result_t<std::string>
            from_hex(const std::string_view _a_str);
__constexpr std::string
            from_hex_with_exception(const std::string_view _a_str);
__no_constexpr std::string
remove_whitespace(const std::string_view _a_str) noexcept;
_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
__constexpr_imp std::string
                create_string(
                    const std::vector<std::string_view>& _a_strs
                ) noexcept
{
    using namespace std;
    string _l_rv{};
    for (auto&& _l_s : _a_strs)
    {
        _l_rv.append(_l_s);
    }
    return _l_rv;
}

__constexpr_imp std::vector<std::string>
                split_string(
                    const std::string_view _a_str,
                    const std::string_view _a_delimiter
                ) noexcept
{
    using namespace std;
    if (_a_str == "")
    {
        return {};
    }
    size_t         pos_start = 0, pos_end, delim_len = _a_delimiter.size();
    string         token;
    vector<string> res;

    while ((pos_end = _a_str.find(_a_delimiter, pos_start)) != string::npos)
    {
        token     = _a_str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(string(_a_str.substr(pos_start)));
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
    size_t         pos_start = 0, pos_end, delim_len = _a_delimiter.size();
    string_view         token;
    vector<string_view> res;

    while ((pos_end = _a_str.find(_a_delimiter, pos_start)) != string::npos)
    {
        token = _a_str.substr(pos_start, pos_end - pos_start);
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

__constexpr_imp const std::string_view
                      return_str_if_next_index_in_bound(
                          const std::size_t      _a_index_to_check,
                          const std::size_t      _a_max_index,
                          const std::string_view _a_str
                      ) noexcept
{
    return _a_index_to_check + 1 < _a_max_index ? _a_str : "";
}

__constexpr_imp std::string
                to_hex(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    vector<unsigned char> _l_chars;
    for (char _l_char : _a_str)
    {
        _l_chars.push_back(static_cast<unsigned char>(_l_char));
    }
    string _l_rv;
    for (unsigned char _l_char : _l_chars)
    {
        _l_rv.append(fmt::format("{:x}", _l_char));
    }
    return _l_rv;
}

__constexpr_imp result_t<std::string>
                from_hex(
                    const std::string_view _a_str
                )
{
    using namespace utility::str;
    using namespace std;
    if ((_a_str.size() % 2) != 0)
    {
        return unexpected(
            fmt::format("Compressed repetition_tree_t is in the form a "
                        "string in hex. As "
                        "each hex digit requires two bytes, the total size "
                        "of the string "
                        "must be divisble by 2. This string is not")
        );
    }
    else
    {
        std::string _l_str{};

        // Parse the bytes.
        const char*           _l_position = _a_str.data();
        vector<unsigned char> _l_chars;
        for (size_t _l_idx{0}; _l_idx < (_a_str.size() / 2); ++_l_idx)
        {
            unsigned char value;
            auto [_l_ptr, _l_ec]
                = from_chars(_l_position, _l_position + 2, value, 16);
            _l_position += 2;
            // If no error, pass the byte to the list of chars
            if (_l_ec == errc())
            {
                _l_chars.push_back(value);
            }
            else
            {
                // Else return failure
                return unexpected{fmt::format(
                    "Could not parse byte pair {0}{1}",
                    *_l_position,
                    *(_l_position + 1)
                )};
            }
        }
        return string(_l_chars.begin(), _l_chars.end());
    }
}

__constexpr_imp std::string
                from_hex_with_exception(
                    const std::string_view _a_str
                )
{
    using namespace std;
    result_t<string> _l_res{from_hex(_a_str)};
    if (not _l_res.has_value())
    {
        throw errors::test_library_exception_t(fmt::format(
            "Could not parse \"{0}\" from hex. Reason given is \"{1}\".",
            _a_str,
            _l_res.error()
        ));
    }
    else
    {
        return _l_res.value();
    }
}

__no_constexpr_imp std::string
                remove_whitespace(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    string _l_rv{_a_str};
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