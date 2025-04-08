#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include <string>
#include "abc_test/utility/types.hpp"

_BEGIN_ABC_NS
__constexpr result_t<std::string>
convert_u8string_to_string(const std::u8string_view _a_str) noexcept;
__constexpr std::string
pack_u8string_into_string(const std::u8string_view _a_str) noexcept;
__constexpr result_t<std::u8string>
convert_string_to_u8string(const std::string_view _a_str) noexcept;
__constexpr std::u8string
unpack_string_to_u8string(const std::string_view _a_str) noexcept;
template <typename T>
__constexpr    std::u8string
type_id() noexcept;
__no_constexpr std::u8string
convert_u32string_to_u8string(const std::u32string_view _a_str) noexcept;
__no_constexpr std::u32string
convert_u8string_to_u32string(const std::u8string_view _a_str) noexcept;
__no_constexpr result_t<std::string>
convert_u32string_to_string(const std::u32string_view _a_str) noexcept;
__no_constexpr std::u16string
convert_u32string_to_u16string(const std::u32string_view _a_str) noexcept;
_END_ABC_NS


_BEGIN_ABC_NS

__constexpr_imp result_t<std::string>
convert_u8string_to_string(
    const std::u8string_view _a_str
) noexcept
{
    using namespace std;
    string _l_utf8_encoded{ pack_u8string_into_string(_a_str) };

    string _l_output;

    // Use utf8cpp to iterate over code points
    string::iterator it = _l_utf8_encoded.begin();
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
                    static_cast<char32_t>(_l_char),
                    std::back_inserter(_l_char_str)
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

__no_constexpr_imp std::u8string
convert_u32string_to_u8string(
    const std::u32string_view _a_str
) noexcept
{
    using namespace std;
    u8string _l_object_as_u8str;
    utf8::utf32to8(
        _a_str.begin(), _a_str.end(), std::back_inserter(_l_object_as_u8str)
    );
    return _l_object_as_u8str;
}

__no_constexpr_imp std::u32string
convert_u8string_to_u32string(
    const std::u8string_view _a_str
) noexcept
{
    using namespace std;
    u32string _l_object_as_u8str;
    utf8::utf8to32(
        _a_str.begin(), _a_str.end(), std::back_inserter(_l_object_as_u8str)
    );
    return _l_object_as_u8str;
}

__no_constexpr_imp result_t<std::string>
convert_u32string_to_string(
    const std::u32string_view _a_str
) noexcept
{
    using namespace std;
    std::string _l_output;

    // Encode each ASCII character as UTF-8 using utf8cpp
    for (char32_t _l_char : _a_str)
    {
        if (static_cast<uint32_t>(_l_char) > 0x7F)
        {
            return unexpected(fmt::format(
                u8"Non-ASCII character encountered. Character = {0}",
                convert_u32string_to_u8string(u32string(1, _l_char))
            ));
        }
        else
        {
            _l_output.push_back(static_cast<char>(_l_char));
        }
    }
    return _l_output;
}
__no_constexpr_imp std::u16string
convert_u32string_to_u16string(const std::u32string_view _a_str) noexcept
{
    using namespace std;
    u8string _l_object_as_u8str;
    utf8::utf32to8(
        _a_str.begin(), _a_str.end(), std::back_inserter(_l_object_as_u8str)
    );
    u16string _l_object_as_u16str;
    utf8::utf8to16(_l_object_as_u8str.begin(), _l_object_as_u8str.end(),
        std::back_inserter(_l_object_as_u16str));
    return _l_object_as_u16str;
}
_END_ABC_NS