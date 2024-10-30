#pragma once

#include "abc_test/internal/utility/internal/macros.h"
#include "abc_test/internal/utility/str/rw_info.h"

#include <filesystem>

_BEGIN_ABC_UTILITY_IO_NS

template <typename T>
struct general_data_with_rw_info_t
{
public:
    __constexpr
        general_data_with_rw_info_t()
        = delete;
    __constexpr
        general_data_with_rw_info_t(
            const std::filesystem::path& _a_path,
            const _ABC_NS_UTILITY::str::rw_info_t<T>& _a_rw_info
        ) noexcept;
    __constexpr const std::filesystem::path&
        path() const noexcept;
    __constexpr const _ABC_NS_UTILITY::str::rw_info_t<T>&
        rw_info() const noexcept;
private:
    std::filesystem::path              _m_path;
    _ABC_NS_UTILITY::str::rw_info_t<T> _m_rw_info;
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
template <typename T>
__no_constexpr abc::utility::io::general_data_with_rw_info_t<T>
general_data_file(
    const std::string_view                 _a_str,
    const abc::utility::str::rw_info_t<T>& _a_rw_info
) noexcept;

_END_ABC_NS

_BEGIN_ABC_UTILITY_IO_NS
template <typename T>
__constexpr_imp
general_data_with_rw_info_t<T>::general_data_with_rw_info_t(
    const std::filesystem::path& _a_path,
    const _ABC_NS_UTILITY::str::rw_info_t<T>& _a_rw_info
) noexcept
    : _m_path(_a_path), _m_rw_info(_a_rw_info)
{}

template <typename T>
__constexpr_imp const std::filesystem::path&
general_data_with_rw_info_t<T>::path() const noexcept
{
    return _m_path;
}

template <typename T>
__constexpr_imp const _ABC_NS_UTILITY::str::rw_info_t<T>&
general_data_with_rw_info_t<T>::rw_info() const noexcept
{
    return _m_rw_info;
}

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
template <typename T>
__no_constexpr_imp abc::utility::io::general_data_with_rw_info_t<T>
general_data_file(
    const std::string_view                 _a_str,
    const abc::utility::str::rw_info_t<T>& _a_rw_info
) noexcept
{
    using namespace std;
    using namespace abc::utility::io;
    return general_data_with_rw_info_t<T>{_a_str, _a_rw_info};
}

_END_ABC_NS