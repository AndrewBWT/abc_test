#pragma once

#include "abc_test/core/ds/test_data/invoked_test_data.hpp"
#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/io/io_utilities.hpp"
#include "abc_test/utility/str/rw_info.hpp"

#include <filesystem>

_BEGIN_ABC_UTILITY_IO_NS

enum class enum_file_type_e
{
    general_data_file,
    tertiary_data_file
};

template <enum_file_type_e File_Type>
struct generic_data_file_t
{
public:
    __constexpr const std::filesystem::path&
                      path() const noexcept;
    static generic_data_file_t<File_Type>
        mk_file_type(
            const std::string_view   _a_str,
            const std::u8string_view _a_extension
        ) noexcept;
    static generic_data_file_t<File_Type>
        mk_file_type(
            const std::string_view   _a_folder,
            const std::string_view   _a_str,
            const std::u8string_view _a_extension
        ) noexcept;
    static generic_data_file_t<File_Type>
        mk_file_type_from_path(const std::filesystem::path& _a_path) noexcept;
private:
    std::filesystem::path _m_path;
    __constexpr
    generic_data_file_t(const std::filesystem::path& _a_path) noexcept;
    static generic_data_file_t<File_Type>
        mk_file_type(
            const std::filesystem::path& _a_path,
            const bool                   _a_has_drive_letter
        ) noexcept;
    static generic_data_file_t<File_Type>
        mk_file_type(
            const std::optional<std::string_view>& _a_folder,
            const std::string_view                 _a_str,
            const std::u8string_view               _a_extension
        ) noexcept;
};

using general_data_file_t
    = generic_data_file_t<enum_file_type_e::general_data_file>;
using tertiary_data_file_t
    = generic_data_file_t<enum_file_type_e::tertiary_data_file>;
using general_data_t = general_data_file_t;
using tertiary_data_t = tertiary_data_file_t;
_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS

__no_constexpr abc::utility::io::general_data_file_t
               gdf(const std::string_view _a_str) noexcept;
__no_constexpr abc::utility::io::general_data_file_t
               gdf(const std::string_view _a_folder,
                   const std::string_view _a_str) noexcept;
__no_constexpr abc::utility::io::general_data_file_t
               gdf_from_path(const std::filesystem::path& _a_path) noexcept;
__no_constexpr abc::utility::io::tertiary_data_file_t
               tdf(const std::string_view _a_str) noexcept;
__no_constexpr abc::utility::io::tertiary_data_file_t
               tdf(const std::string_view _a_folder,
                   const std::string_view _a_str) noexcept;
__no_constexpr abc::utility::io::tertiary_data_file_t
               tdf_from_path(const std::filesystem::path& _a_path) noexcept;
_END_ABC_NS

_BEGIN_ABC_UTILITY_IO_NS

template <enum_file_type_e File_Type>
__constexpr_imp const std::filesystem::path&
                      generic_data_file_t<File_Type>::path() const noexcept
{
    return _m_path;
}

template <enum_file_type_e File_Type>
inline generic_data_file_t<File_Type>
    generic_data_file_t<File_Type>::mk_file_type(
        const std::string_view   _a_str,
        const std::u8string_view _a_extension
    ) noexcept
{
    return generic_data_file_t<File_Type>::mk_file_type(
        std::nullopt, _a_str, _a_extension
    );
}

template <enum_file_type_e File_Type>
inline generic_data_file_t<File_Type>
    generic_data_file_t<File_Type>::mk_file_type(
        const std::string_view   _a_folder,
        const std::string_view   _a_str,
        const std::u8string_view _a_extension
    ) noexcept
{
    return generic_data_file_t<File_Type>::mk_file_type(
        std::make_optional(_a_folder), _a_str, _a_extension
    );
}

template <enum_file_type_e File_Type>
inline generic_data_file_t<File_Type>
    generic_data_file_t<File_Type>::mk_file_type_from_path(
        const std::filesystem::path& _a_path
    ) noexcept
{
    return generic_data_file_t<File_Type>::mk_file_type(_a_path, true);
}

template <enum_file_type_e File_Type>
__constexpr_imp
    generic_data_file_t<File_Type>::generic_data_file_t(
        const std::filesystem::path& _a_path
    ) noexcept
    : _m_path(_a_path)
{}

template <enum_file_type_e File_Type>
inline generic_data_file_t<File_Type>
    generic_data_file_t<File_Type>::mk_file_type(
        const std::filesystem::path& _a_path,
        const bool                   _a_has_drive_letter
    ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<File_Type>(
        normalise_for_file_use(_a_path.u8string(), _a_has_drive_letter)
    );
}

template <enum_file_type_e File_Type>
inline generic_data_file_t<File_Type>
    generic_data_file_t<File_Type>::mk_file_type(
        const std::optional<std::string_view>& _a_folder,
        const std::string_view                 _a_str,
        const std::u8string_view               _a_extension
    ) noexcept
{
    using namespace _ABC_NS_GLOBAL;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace std;
    auto _l_path{filesystem::absolute(get_this_threads_current_test().path())};
    if (_a_folder.has_value())
    {
        _l_path /= _a_folder.value();
    }
    _l_path /= _a_str;
    _l_path.concat(fmt::format(".{0}", cast_u8string_to_string(_a_extension)));
    return generic_data_file_t<File_Type>::mk_file_type(_l_path, true);
}

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_NS
__no_constexpr_imp _ABC_NS_UTILITY_IO::general_data_file_t
                   gdf(
                       const std::string_view _a_str
                   ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<enum_file_type_e::general_data_file>::
        mk_file_type(
            _a_str,
            global::get_this_threads_test_options()
                .individual_io_based_test_options.general_data_extension
        );
}

__no_constexpr_imp _ABC_NS_UTILITY_IO::general_data_file_t
                   gdf(
                       const std::string_view _a_folder,
                       const std::string_view _a_str
                   ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<enum_file_type_e::general_data_file>::
        mk_file_type(
            _a_folder,
            _a_str,
            global::get_this_threads_test_options()
                .individual_io_based_test_options.general_data_extension
        );
}

__no_constexpr_imp _ABC_NS_UTILITY_IO::general_data_file_t
                   gdf_from_path(
                       const std::filesystem::path& _a_path
                   ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<
        enum_file_type_e::general_data_file>::mk_file_type_from_path(_a_path);
}

__no_constexpr_imp _ABC_NS_UTILITY_IO::tertiary_data_file_t
                   tdf(
                       const std::string_view _a_str
                   ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<enum_file_type_e::tertiary_data_file>::
        mk_file_type(
            _a_str,
            global::get_this_threads_test_options()
                .individual_io_based_test_options.tertiary_data_file_extension
        );
}

__no_constexpr_imp _ABC_NS_UTILITY_IO::tertiary_data_file_t
                   tdf(
                       const std::string_view _a_folder,
                       const std::string_view _a_str
                   ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<enum_file_type_e::tertiary_data_file>::
        mk_file_type(
            _a_folder,
            _a_str,
            global::get_this_threads_test_options()
                .individual_io_based_test_options.tertiary_data_file_extension
        );
}

__no_constexpr_imp _ABC_NS_UTILITY_IO::tertiary_data_file_t
                   tdf_from_path(
                       const std::filesystem::path& _a_path
                   ) noexcept
{
    using namespace _ABC_NS_UTILITY_IO;
    return generic_data_file_t<
        enum_file_type_e::tertiary_data_file>::mk_file_type_from_path(_a_path);
}

_END_ABC_NS