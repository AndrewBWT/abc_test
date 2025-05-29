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
template<typename T>
using file_data_name_t = std::variant< general_data_with_rw_info_t<T>, general_data_t,std::string>;
template<typename T>
using file_data_names_t = std::vector<file_data_name_t<T>>;
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