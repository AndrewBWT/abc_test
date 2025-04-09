#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/types.hpp"

#include <string>

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
__no_constexpr std::string
    unsafe_convert_u32string_to_string(const std::u32string_view _a_str
    ) noexcept;
__no_constexpr std::u16string
    convert_u32string_to_u16string(const std::u32string_view _a_str) noexcept;
__no_constexpr std::u8string
               positive_integer_to_placement(const std::size_t _a_number);
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
        for (size_t _l_idx{ 1 }; it != end; ++_l_idx)
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
                    u8"The function convert_u8string_to_string could not encode "
                    u8"the character '{0}' into basic ASCII, which is the encoding "
                    u8"used for the std::string type in abc_test. This error "
                    u8"occoured when converting the {1} character in the string "
                    u8"\"{2}\".",
                    convert_u32string_to_u8string(u32string(1, _l_char)),
                    positive_integer_to_placement(_l_idx),
                    _a_str
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
    for (size_t _l_idx{1}; char32_t _l_char : _a_str)
    {
        if (static_cast<uint32_t>(_l_char) > 0x7F)
        {
            return unexpected(fmt::format(
                u8"The function convert_u8string_to_string could not encode "
                u8"the character '{0}' into basic ASCII, which is the encoding "
                u8"used for the std::string type in abc_test. This error "
                u8"occoured when converting the {1} character in the string "
                u8"\"{2}\".",
                convert_u32string_to_u8string(u32string(1, _l_char)),
                positive_integer_to_placement(_l_idx),
                convert_u32string_to_u8string(_a_str)
            ));
        }
        else
        {
            _l_output.push_back(static_cast<char>(_l_char));
            ++_l_idx;
        }
    }
    return _l_output;
}

__no_constexpr_imp std::string
                   unsafe_convert_u32string_to_string(
                       const std::u32string_view _a_str
                   ) noexcept
{
    using namespace std;
    std::string _l_output;

    // Encode each ASCII character as UTF-8 using utf8cpp
    for (char32_t _l_char : _a_str)
    {
        _l_output.push_back(static_cast<char>(_l_char));
    }
    return _l_output;
}

__no_constexpr_imp std::u16string
                   convert_u32string_to_u16string(
                       const std::u32string_view _a_str
                   ) noexcept
{
    using namespace std;
    u8string _l_object_as_u8str;
    utf8::utf32to8(
        _a_str.begin(), _a_str.end(), std::back_inserter(_l_object_as_u8str)
    );
    u16string _l_object_as_u16str;
    utf8::utf8to16(
        _l_object_as_u8str.begin(),
        _l_object_as_u8str.end(),
        std::back_inserter(_l_object_as_u16str)
    );
    return _l_object_as_u16str;
}

__no_constexpr_imp std::u8string
                   positive_integer_to_placement(
                       const std::size_t _a_number
                   )
{
    switch (_a_number)
    {
    case 0:
        throw std::runtime_error("");
    case 1:
        return u8"1st";
    case 2:
        return u8"2nd";
    case 3:
        return u8"3rd";
    default:
        return fmt::format(u8"{0}th", _a_number);
    }
}

_END_ABC_NS