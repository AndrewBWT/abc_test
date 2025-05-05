#pragma once
#include "abc_test/core/global.hpp"
#include "abc_test/utility/io/file/general_data.hpp"
#include "abc_test/utility/io/file/general_data_with_rw_info.hpp"
#include "abc_test/utility/io/file/tertiary_data.hpp"

_BEGIN_ABC_UTILITY_IO_NS
template <typename T>
using file_name_t = std::
    variant<general_data_with_rw_info_t<T>, general_data_t, tertiary_data_t>;
template <typename T>
using file_names_t = std::vector<file_name_t<T>>;
template <typename T>
__constexpr const std::filesystem::path&
                  path(const file_name_t<T>& _a_file_name);

template <typename T>
__constexpr std::optional<abc::utility::str::rw_info_t<T>>
            opt_rw_info(const file_name_t<T>& _a_file_name);
template <typename T>
__constexpr std::string
            generate_comment_str(
                const file_name_t<T>& _a_file_name,
                const std::type_info& _a_type_info
            );

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
        {u8"'",  u8"_"}
    };
    u8string _l_rv(_a_str);
    for (auto& [_l_to_find, _l_to_replace_with] : _l_strs_to_replace)
    {
        bool _l_replaced{false};
        do
        {
            _l_replaced = false;
            if (auto _l_str_pos{_l_rv.find(_l_to_find)};
                _l_str_pos != u8string::npos)
            {
                _l_replaced = true;
                _l_rv.replace(
                    _l_str_pos, _l_to_find.size(), _l_to_replace_with
                );
            }
        }
        while (_l_replaced);
    }
    vector<pair<u8string, u8string>> _l_strs_to_replace_if_not_first = {
        {u8":", u8"_"}
    };
    for (auto& [_l_to_find, _l_to_replace_with] :
         _l_strs_to_replace_if_not_first)
    {
        bool _l_replaced{false};
        do
        {
            _l_replaced = false;
            const size_t _l_first_pos{_l_rv.find(_l_to_find)};
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
        }
        while (_l_replaced);
    }
    return _l_rv;
}

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
template <typename T>
__constexpr_imp const std::filesystem::path&
                      path(
                          const file_name_t<T>& _a_file_name
                      )
{
    using namespace std;
    if (auto _l_ptr{get_if<general_data_with_rw_info_t<T>>(&_a_file_name)};
        _l_ptr != nullptr)
    {
        return _l_ptr->general_data_file().path();
    }
    else if (auto _l_ptr{get_if<general_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return _l_ptr->path();
    }
    else if (auto _l_ptr{get_if<tertiary_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return _l_ptr->path();
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_file_name);
    }
}

template <typename T>
__constexpr_imp std::optional<abc::utility::str::rw_info_t<T>>
                opt_rw_info(
                    const file_name_t<T>& _a_file_name
                )
{
    using namespace std;
    using namespace abc::utility::str;
    if (auto _l_ptr{get_if<general_data_with_rw_info_t<T>>(&_a_file_name)};
        _l_ptr != nullptr)
    {
        return optional<rw_info_t<T>>(_l_ptr->rw_info());
    }
    else if (auto _l_ptr{get_if<general_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return optional<rw_info_t<T>>(rw_info_t<T>{});
    }
    else if (auto _l_ptr{get_if<tertiary_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return optional<rw_info_t<T>>();
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_file_name);
    }
}

template <typename T>
__constexpr_imp std::string
                generate_comment_str(
                    const file_name_t<T>& _a_file_name,
                    const std::type_info& _a_type_info
                )
{
    using namespace std;
    using namespace abc::utility::str;
    if (auto _l_ptr{get_if<general_data_with_rw_info_t<T>>(&_a_file_name)};
        _l_ptr != nullptr)
    {
        return typeid(T).name();
    }
    else if (auto _l_ptr{get_if<general_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return typeid(T).name();
    }
    else if (auto _l_ptr{get_if<tertiary_data_t>(&_a_file_name)};
             _l_ptr != nullptr)
    {
        return _a_type_info.name();
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(_a_file_name);
    }
}

_END_ABC_UTILITY_IO_NS