#pragma once
#include "abc_test/external/utfcpp/utf8.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <cuchar>
#include <expected>
#include <string>
#include <vector>

#include <fmt/xchar.h>

_BEGIN_ABC_NS

template <typename T>
using result_t = std::expected<T, std::u8string>;

__constexpr result_t<std::string>
convert_u8string_to_string(
    const std::u8string_view _a_str
) noexcept;
__constexpr std::string
pack_u8string_into_string(
    const std::u8string_view _a_str
) noexcept;
__constexpr result_t<std::u8string>
convert_string_to_u8string(
    const std::string_view _a_str
) noexcept;
__constexpr std::u8string
unpack_string_to_u8string(
    const std::string_view _a_str
) noexcept;
template <typename T>
__constexpr std::u8string
type_id() noexcept;
_END_ABC_NS

_BEGIN_ABC_NS

__constexpr_imp result_t<std::string>
            convert_u8string_to_string(
                const std::u8string_view _a_str
            ) noexcept
{
    using namespace std;
    string _l_utf8_encoded{pack_u8string_into_string(_a_str)};

    string _l_output;

    // Use utf8cpp to iterate over code points
    string::iterator it  = _l_utf8_encoded.begin();
    string::iterator end = _l_utf8_encoded.end();

    try
    {
        while (it != end)
        {
            char32_t _l_char = utf8::next(it, end);
            if (not (_l_char <= 0x7F))
            {
                u8string _l_char_str{};
                utf8::append(
                    static_cast<char32_t>(_l_char), std::back_inserter(_l_char_str)
                );
                return unexpected(fmt::format(
                    u8"Non-ASCII character encountered. Character = {0}",
                    _l_char_str
                ));
            }
            else
            {
                _l_output.push_back(static_cast<char>(_l_char));
            }
        }
    }
    catch (const utf8::invalid_utf8& _l_exception)
    {
        return unexpected(fmt::format(
            u8"Invalid UTF - 8 exception encountered. {0}", 
            unpack_string_to_u8string(_l_exception.what())
        ));
    }
    return _l_output;
}

__constexpr_imp std::string
            pack_u8string_into_string(
                const std::u8string_view _a_str
            ) noexcept
{
    return std::string(_a_str.begin(), _a_str.end());
}

__constexpr_imp result_t<std::u8string>
            convert_string_to_u8string(
                const std::string_view _a_str
            ) noexcept
{
    using namespace std;
    std::u8string _l_output;

    // Encode each ASCII character as UTF-8 using utf8cpp
    for (char _l_char : _a_str)
    {
        if (static_cast<unsigned char>(_l_char) > 0x7F)
        {
            return unexpected(fmt::format(
                u8"Non-ASCII character encountered. Character = {0}", _l_char
            ));
        }
        utf8::append(
            static_cast<char32_t>(_l_char), std::back_inserter(_l_output)
        );
    }
    return _l_output;
}

__constexpr_imp std::u8string
            unpack_string_to_u8string(
                const std::string_view _a_str
            ) noexcept
{
    return std::u8string(_a_str.begin(), _a_str.end());
}

template <typename T>
__constexpr_imp std::u8string
            type_id() noexcept
{
    return convert_string_to_u8string(typeid(T).name()).value();
}

_END_ABC_NS