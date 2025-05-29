#pragma once

#include <string>

#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include <vector>


_BEGIN_ABC_UTILITY_IO_NS
__constexpr std::u8string
normalise_for_file_use(
    const std::u8string_view _a_str
) noexcept
{
    using namespace std;
    vector<pair<u8string, u8string>> _l_strs_to_replace = {
        // {u8":",  u8"_"},
        {u8" ",  u8"_"},
        {u8"\"", u8"" },
        {u8"`",  u8"_"},
        {u8"'",  u8"_"},
        {u8"<",  u8"_"},
        {u8">",  u8"_"},
        {u8",",  u8"_"}
    };
    u8string _l_rv(_a_str);
    for (auto& [_l_to_find, _l_to_replace_with] : _l_strs_to_replace)
    {
        bool _l_replaced{ false };
        do
        {
            _l_replaced = false;
            if (auto _l_str_pos{ _l_rv.find(_l_to_find) };
                _l_str_pos != u8string::npos)
            {
                _l_replaced = true;
                _l_rv.replace(
                    _l_str_pos, _l_to_find.size(), _l_to_replace_with
                );
            }
        } while (_l_replaced);
    }
    vector<pair<u8string, u8string>> _l_strs_to_replace_if_not_first = {
        {u8":", u8"_"}
    };
    for (auto& [_l_to_find, _l_to_replace_with] :
        _l_strs_to_replace_if_not_first)
    {
        bool _l_replaced{ false };
        do
        {
            _l_replaced = false;
            const size_t _l_first_pos{ _l_rv.find(_l_to_find) };
            if (_l_first_pos == u8string::npos)
            {
                continue;
            }
            else
            {
                const size_t _l_second_pos{
                    _l_rv.find(_l_to_find, _l_first_pos + 1)
                };
                if (_l_second_pos == u8string::npos)
                {
                    continue;
                }
                else
                {
                    _l_replaced = true;
                    _l_rv.replace(
                        _l_second_pos, _l_to_find.size(), _l_to_replace_with
                    );
                }
            }
        } while (_l_replaced);
    }
    return _l_rv;
}

_END_ABC_UTILITY_IO_NS