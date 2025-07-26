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
    file_line_writer_t() noexcept = default;
    __no_constexpr void
        write_comment(const std::u8string_view _a_str_to_write) noexcept;
    __no_constexpr void
        write_line(const std::u8string_view _a_str_to_write) noexcept;
private:
    size_t                          _m_current_line_idx;
    std::shared_ptr<std::wofstream> _m_file_handler;
    std::filesystem::path           _m_file_name;
    // const test_options_base_t* _m_test_options;
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
__no_constexpr_imp file_line_writer_t::~file_line_writer_t()
{}

__no_constexpr_imp
    file_line_writer_t::file_line_writer_t(
        const std::filesystem::path& _a_file_name
    )

    : _m_current_line_idx(0)
    , _m_file_handler(std::shared_ptr<std::wofstream>())
    , _m_file_name(normalise_for_file_use(_a_file_name.u8string(), true))
{
    using namespace std;
    using namespace std::filesystem;
    using namespace errors;
    std::wstring path_to_open = _m_file_name.native();
    if (path_to_open.rfind(L"\\\\?\\", 0) == 0 && path_to_open.size() < 260)
    {
        path_to_open = path_to_open.substr(4);
    }
    _m_file_handler = make_shared<wofstream>(path_to_open, std::ios_base::app);
    if (not _m_file_handler.get()->is_open())
    {
        auto                  fn      = std::filesystem::path(path_to_open);
        std::filesystem::path current = fn.parent_path();
        while (current.native().size() > 5)
        {
            std::wcout << L"Characters in directory name:\n";
            for (wchar_t ch : current.native())
            {
                std::wcout << L"'" << ch << L"' : " << static_cast<int>(ch)
                           << std::endl;
            }
            std::wcout << current.native() << L" : "
                       << (std::filesystem::exists(current) ? L"Exists"
                                                            : L"Missing")
                       << std::endl;
            current = current.parent_path();
        }
        std::wcout << L"Trying to read: " << fn.native() << std::endl;
        std::wcout << L"Length: " << fn.native().size() << std::endl;
        throw test_library_exception_t(fmt::format(
            u8"Unable to open file_line_writer_t object as file \"{0}\", "
            u8"even though all checks suggest we should be able to.",
            _m_file_name
        ));
    }
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
        u8"{0}{1}",
        global::get_this_threads_test_options().individual_io_based_test_options.comment_str,
        _a_str_to_write
    ));
}

__no_constexpr_imp void
    file_line_writer_t::write_line(
        const std::u8string_view _a_str_to_write
    ) noexcept
{
    using namespace std;
    if (global::get_this_threads_test_options().individual_io_based_test_options.write_data_to_files)
    {
        wofstream& _l_out{*_m_file_handler.get()};
        auto _l_wstring{ abc::utility::str::unicode_conversion<wchar_t>(_a_str_to_write) };
        if (_l_wstring.has_value())
        {
            _l_out << _l_wstring.value() << std::endl;;
        }
        else
        {
            _l_out << "Couldn't work it out" << std::endl;
        }
    }
}

_END_ABC_UTILITY_IO_NS
