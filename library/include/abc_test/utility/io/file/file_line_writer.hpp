#pragma once

#include "abc_test/utility/io/file/file_name.hpp"

_BEGIN_ABC_UTILITY_IO_NS

struct file_line_writer_t
{
public:
    __no_constexpr ~file_line_writer_t();
    __no_constexpr
        file_line_writer_t(const std::filesystem::path& _a_file_name
                           //		const test_options_base_t* _a_test_options
        );
    __constexpr
    file_line_writer_t() noexcept
        = default;
    __no_constexpr void
        write_comment(const std::u8string_view _a_str_to_write) noexcept;
    __no_constexpr void
        write_line(const std::u8string_view _a_str_to_write) noexcept;
private:
    size_t                         _m_current_line_idx;
    std::shared_ptr<std::ofstream> _m_file_handler;
    std::filesystem::path          _m_file_name;
    // const test_options_base_t* _m_test_options;
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
__no_constexpr_imp file_line_writer_t::~file_line_writer_t()
{
}
__no_constexpr_imp
    file_line_writer_t::file_line_writer_t(
        const std::filesystem::path& _a_file_name
        //	const test_options_base_t* _a_test_options
    )
    : _m_file_handler(std::shared_ptr<std::ofstream>())
    , _m_current_line_idx(0)
    , _m_file_name(_a_file_name)
//	, _m_test_options(_a_test_options)
{
    using namespace std;
    using namespace std::filesystem;
    using namespace errors;
    // if ()
    // if (_m_file_name.is_valid())
    {
        _m_file_handler
            = make_shared<ofstream>(_m_file_name, std::ios_base::app);
        if (not _m_file_handler.get()->is_open())
        {
            throw test_library_exception_t(fmt::format(
                u8"Unable to open file_line_writer_t object as file \"{0}\", "
                u8"even though all checks suggest we should be able to.",
                _m_file_name
            ));
        }
    }
    // else
    //{
    //	throw test_library_exception_t(
    //		fmt::format("file name \"{0}\" has been flagged as being invalid.",
    //				_m_file_name.file_path()));
    //	}
}

/*__constexpr_imp
    file_line_writer_t::file_line_writer_t(
    ) noexcept
    : _m_file_handler(std::unique_ptr<std::ofstream>(nullptr))
    , _m_current_line_idx(0)
    , _m_file_name(file_name_with_extension_t{})
    , _m_test_options(nullptr)
{

}*/
__no_constexpr_imp void
    file_line_writer_t::write_comment(
        const std::u8string_view _a_str_to_write
    ) noexcept
{
    write_line(fmt::format(
        u8"{0}{1}", global::get_global_test_options().comment_str, _a_str_to_write
    ));
}

__no_constexpr_imp void
    file_line_writer_t::write_line(
        const std::u8string_view _a_str_to_write
    ) noexcept
{
    using namespace std;
    if (global::get_global_test_options().write_data_to_files)
    {
        ofstream& _l_out{ *_m_file_handler.get() };
        _l_out << string(_a_str_to_write.begin(), _a_str_to_write.end()) << std::endl;
    }
}

_END_ABC_UTILITY_IO_NS