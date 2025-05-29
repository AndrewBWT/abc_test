#pragma once

#include "abc_test/core/ds/test_data/invoked_test_data.hpp"
#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/rw_info.hpp"
#include "abc_test/utility/io/io_utilities.hpp"

#include <filesystem>

_BEGIN_ABC_UTILITY_IO_NS

struct general_data_t
{
public:
    __constexpr
    general_data_t(const std::filesystem::path& _a_path) noexcept;
    __constexpr const std::filesystem::path&
                      path() const noexcept;
private:
    std::filesystem::path _m_path;
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
__no_constexpr abc::utility::io::general_data_t
               gdf(const std::string_view _a_str) noexcept;
__no_constexpr abc::utility::io::general_data_t
               gdf(const std::string_view _a_folder,
                   const std::string_view _a_str) noexcept;
__no_constexpr abc::utility::io::general_data_t
               gdf_from_path(const std::filesystem::path& _a_path) noexcept;
_END_ABC_NS

_BEGIN_ABC_UTILITY_IO_NS
__constexpr_imp
    general_data_t::general_data_t(
        const std::filesystem::path& _a_path
    ) noexcept
    : _m_path(_a_path)
{}

__constexpr_imp const std::filesystem::path&
                      general_data_t::path() const noexcept
{
    return _m_path;
}

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
__no_constexpr_imp abc::utility::io::general_data_t
                   gdf(
                       const std::string_view _a_str
                   ) noexcept
{
    return gdf_from_path(
        (std::filesystem::absolute(global::get_this_threads_current_test().path()) / _a_str)
            .concat(".").concat(global::get_global_test_options().general_data_extension)
    );
}

__no_constexpr_imp abc::utility::io::general_data_t
                   gdf(
                       const std::string_view _a_folder,
                       const std::string_view _a_str
                   ) noexcept
{
    using namespace std;
    using namespace std::filesystem;
    return gdf_from_path(
        (absolute(global::get_global_test_options().root_path) / _a_folder / _a_str)
            .concat(".")
            .concat(global::get_global_test_options().general_data_extension)
    );
}

__no_constexpr_imp abc::utility::io::general_data_t
                   gdf_from_path(
                       const std::filesystem::path& _a_path
                   ) noexcept
{
    using namespace abc::utility::io;
    return general_data_t(
        abc::utility::io::normalise_for_file_use(
        _a_path.u8string()));
}

_END_ABC_NS