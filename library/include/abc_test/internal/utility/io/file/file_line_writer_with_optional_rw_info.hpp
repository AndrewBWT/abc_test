#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/str/rw_info.hpp"
#include "abc_test/internal/utility/io/file/file_line_writer.hpp"

_BEGIN_ABC_UTILITY_IO_NS

template <typename T>
class file_line_writer_with_optional_rw_info_t : public file_line_writer_t
{
private:
    std::optional<abc::utility::str::rw_info_t<T>> _m_opt_rw_info;
public:
    __no_constexpr
        file_line_writer_with_optional_rw_info_t(
            const utility::io::file_name_t<T>& _a_frw,
            const std::string_view _a_comment_if_new_file
        );
    __constexpr bool
        has_printer_parser() const noexcept;
    __no_constexpr_imp void
        write_line_using_rw_info(
            const T& _a_element
        );
private:
    __no_constexpr
        file_line_writer_with_optional_rw_info_t(
            const utility::io::file_name_t<T>& _a_frw,
            const bool _a_exists,
            const std::string_view _a_comment_if_new_file
        );
};
_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
template <typename T>
__no_constexpr_imp
file_line_writer_with_optional_rw_info_t<T>::file_line_writer_with_optional_rw_info_t(
    const utility::io::file_name_t<T>& _a_frw,
    const std::string_view _a_comment_if_new_file
)
    : file_line_writer_with_optional_rw_info_t(_a_frw,std::filesystem::exists(path(_a_frw)),_a_comment_if_new_file)
{}
template <typename T>
__constexpr_imp bool
file_line_writer_with_optional_rw_info_t<T>::has_printer_parser() const noexcept
{
    return _m_opt_rw_info.has_value();
}
template <typename T>
__no_constexpr_imp void
file_line_writer_with_optional_rw_info_t<T>::write_line_using_rw_info(
    const T& _a_element
)
{
    if (_m_opt_rw_info.has_value())
    {
        return write_line(
            _m_opt_rw_info.value().internal_printer->run_printer(_a_element)
        );
    }
}
template <typename T>
__no_constexpr_imp
file_line_writer_with_optional_rw_info_t<T>::file_line_writer_with_optional_rw_info_t(
    const utility::io::file_name_t<T>& _a_frw,
    const bool _a_exists,
    const std::string_view _a_comment_if_new_file
)
      : file_line_writer_t(path(_a_frw))
      , _m_opt_rw_info(opt_rw_info(_a_frw))
{
    if (not _a_exists)
    {
        write_comment(_a_comment_if_new_file);
}
}
_END_ABC_UTILITY_IO_NS