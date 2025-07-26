#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/rw_info.hpp"

#include <filesystem>

_BEGIN_ABC_UTILITY_IO_NS

struct tertiary_data_t
{
public:
    __constexpr
    tertiary_data_t(const std::filesystem::path& _a_path) noexcept;
    __constexpr const std::filesystem::path&
                      path() const noexcept;
private:
    std::filesystem::path _m_path;
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
template <typename T>
__no_constexpr abc::utility::io::tertiary_data_t
               tdf(const std::string_view _a_str) noexcept;

_END_ABC_NS

_BEGIN_ABC_UTILITY_IO_NS
__constexpr_imp
    tertiary_data_t::tertiary_data_t(
        const std::filesystem::path& _a_path
    ) noexcept
    : _m_path(_a_path)
{}

__constexpr_imp const std::filesystem::path&
                      tertiary_data_t::path() const noexcept
{
    return _m_path;
}

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
__no_constexpr_imp abc::utility::io::tertiary_data_t
                   tdf(
                       const std::string_view _a_str
                   ) noexcept
{
    using namespace std;
    using namespace abc::utility::io;
    return (global::get_this_threads_current_test().path() / _a_str)
        .concat(".")
        .concat(global::get_this_threads_test_options().individual_io_based_test_options.tertiary_data_file_extension);
    return tertiary_data_t(_a_str);
}

_END_ABC_NS