#pragma once
#include "abc_test/internal/global.h"

_BEGIN_ABC_UTILITY_IO_NS

/*!
 * Object for holding file_names. Can be given a folder as well to create a
 * specific file object.
 */
struct file_name_t
{
public:
    __constexpr
    file_name_t() noexcept;
    __constexpr
    file_name_t(
        const std::string_view                  _a_str,
        const std::filesystem::path& _a_folder = std::filesystem::path()
    ) noexcept;
    __constexpr
        file_name_t(
            const char*                  _a_str,
            const std::filesystem::path& _a_folder = std::filesystem::path()
        ) noexcept;
    /*!
     * Checks if the file name is valid. Specificallly checks for things like an
     * empty string, or characters not allowed in a file name.
     */
    __no_constexpr bool
        is_valid() const noexcept;
    /*!
     * Gets the internal file path.
     */
    __constexpr const std::filesystem::path&
                      file_path() const noexcept;
protected:
    std::filesystem::path _m_file_path;
    __constexpr
    file_name_t(const std::filesystem::path& _a_file_path) noexcept;
};

using opt_file_name_t = std::optional<file_name_t>;
_END_ABC_UTILITY_IO_NS

/*!
 * formatter for post_setup_test_ata object.
 */
template <>
struct fmt::formatter<_ABC_NS_UTILITY::io::file_name_t> : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    __no_constexpr auto
        format(_ABC_NS_UTILITY::io::file_name_t _a_fnwe, format_context& _a_cxt)
            const -> format_context::iterator;
};

_BEGIN_ABC_UTILITY_IO_NS
__constexpr_imp
    file_name_t::file_name_t() noexcept
    : file_name_t(std::string_view{}, std::filesystem::path())
{}

__constexpr_imp
    file_name_t::file_name_t(
        const std::string_view                  _a_str,
        const std::filesystem::path& _a_folder
    ) noexcept
    : file_name_t(
        _a_str.empty()
              ? std::filesystem::path{}
              : std::filesystem::path(_a_folder) / (_a_str)
      )
{}

__constexpr_imp
file_name_t::file_name_t(
    const char*                  _a_str,
    const std::filesystem::path& _a_folder
) noexcept
    : file_name_t(
        _a_str == nullptr
        ? std::filesystem::path{}
        : std::filesystem::path(_a_folder) / (_a_str)
    )
{}

__no_constexpr_imp bool
    file_name_t::is_valid() const noexcept
{
    if (_m_file_path.empty())
    {
        return false;
    }
    return true;
}

__constexpr_imp const std::filesystem::path&
                      file_name_t::file_path() const noexcept
{
    return _m_file_path;
}

__constexpr_imp
    file_name_t::file_name_t(
        const std::filesystem::path& _a_file_path
    ) noexcept
    : _m_file_path(_a_file_path)
{}

_END_ABC_UTILITY_IO_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_UTILITY::io::file_name_t>::format(
        _ABC_NS_UTILITY::io::file_name_t _a_fn,
        format_context&                  _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    using namespace _ABC_NS_UTILITY::io;
    string _l_rv{fmt::format(
        "{0}{{_m_file_path = {1}}}",
        typeid(file_name_t).name(),
        _a_fn.file_path().string()
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}
