#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/str/rw_info.hpp"
#include "abc_test/internal/ds/test_data/invoked_test_data.hpp"

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
template <typename T>
__no_constexpr abc::utility::io::general_data_t
general_data_file(
    const std::string_view                 _a_str
) noexcept;

_END_ABC_NS

_BEGIN_ABC_UTILITY_IO_NS
__constexpr_imp
general_data_t::general_data_t(const std::filesystem::path& _a_path) noexcept
    : _m_path(_a_path)
{

}
__constexpr_imp const std::filesystem::path&
general_data_t::path() const noexcept
{
    return _m_path;
}

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
__no_constexpr_imp abc::utility::io::general_data_t
general_data_file(
    const std::string_view                 _a_str
) noexcept
{
    using namespace std;
    using namespace abc::utility::io;
    using namespace std::filesystem;
    return general_data_t(global::get_this_threads_current_test().path() / _a_str);
}

_END_ABC_NS