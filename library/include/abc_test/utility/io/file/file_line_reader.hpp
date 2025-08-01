#pragma once
#include "abc_test/core/options/test_options_base.hpp"

#include <fmt/std.h>
#include <fstream>
#include <iostream>
#include <string>

_BEGIN_ABC_UTILITY_IO_NS

/*!
 * @brief Object used to encapsulate the logic required to read a file line by
 * line.
 */
struct file_line_reader_t
{
public:
    __no_constexpr
        file_line_reader_t(const std::filesystem::path& _a_file_name);
    __no_constexpr
        file_line_reader_t() noexcept
        = default;
    __constexpr const std::u8string
                      current_line() noexcept;
    __constexpr bool
        has_current_line() const noexcept;
    __constexpr bool
                get_next_line();

    __constexpr std::size_t
                line_number() const noexcept
    {
        return _m_current_line_idx + 1;
    }
private:
    size_t                         _m_current_line_idx{0};
    std::u8string                  _m_current_line;
    std::shared_ptr<std::ifstream> _m_file_handler;
    std::filesystem::path          _m_file_name;
    bool                           _m_finished_reading;
};

_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
__no_constexpr_imp
    file_line_reader_t::file_line_reader_t(
        const std::filesystem::path& _a_file_name
    )
    : _m_current_line_idx(0)
    , _m_current_line(u8"")
    , _m_file_handler(std::shared_ptr<std::ifstream>())
    , _m_file_name(normalise_for_file_use(_a_file_name.u8string(), true))
    , _m_finished_reading(false)
{
    using namespace std;
    using namespace std::filesystem;
    using namespace _ABC_NS_ERRORS;
    using std::filesystem::path;
    if (not exists(_m_file_name))
    {
        throw abc_test_exception_t(
            {fmt::format(
                u8"Unable to open file_line_reader_t object as file \"{0}\" "
                u8"does "
                u8"not exist",
                _m_file_name.u8string()
            )},
            false
        );
    }
    _m_file_handler = make_shared<ifstream>(_m_file_name);
    if (_m_file_handler.get()->is_open())
    {
        get_next_line();
    }
    else
    {
        throw abc_test_exception_t(
            {fmt::format(
                u8"Unable to open file_line_reader_t object as file \"{0}\", "
                u8"even though file exists.",
                _m_file_name.u8string()
            )},
            false
        );
    }
}

__constexpr_imp const std::u8string
                      file_line_reader_t::current_line() noexcept
{
    return _m_current_line;
}

__constexpr_imp bool
    file_line_reader_t::has_current_line() const noexcept
{
    return _m_current_line_idx > 0;
}

__constexpr_imp bool
    file_line_reader_t::get_next_line()
{
    using namespace _ABC_NS_ERRORS;
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    bool _l_exit{true};
    if (_m_finished_reading)
    {
        return false;
    }
    ifstream& _l_file_hander{*_m_file_handler.get()};
    do
    {
        ++_m_current_line_idx;
        string _l_unprocessed_line;
        std::getline(_l_file_hander, _l_unprocessed_line);
        _m_current_line = cast_string_to_u8string(_l_unprocessed_line);
        const bool _l_error_reading_file{
            _l_file_hander.fail() || _l_file_hander.bad()
        };
        bool _l_eof_found{_l_file_hander.eof()};
        if (_l_eof_found)
        {
            _m_finished_reading = true;
        }
        if (_m_finished_reading == true && _m_current_line.empty())
        {
            // eof found, empty line.
            break;
        }
        else if (_l_error_reading_file)
        {
            // Some other reading error.
            _m_current_line     = u8"";
            _m_current_line_idx = 0;
            _m_finished_reading = true;
            throw abc_test_exception_t(
                {fmt::format(
                    u8"Error encountered reading line {0} in file \"{1}\". "
                    u8"The fail bit of the file was set to {2} and the bad_bit "
                    u8"of "
                    u8"the file was set to {3}",
                    _m_current_line_idx,
                    _m_file_name,
                    _l_file_hander.fail(),
                    _l_file_hander.bad()
                )},
                false
            );
        }
        else if (_m_current_line.starts_with(
                     global::get_this_threads_test_options()
                         .individual_io_based_test_options.comment_str
                 ))
        {
            if (_m_finished_reading)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            return true;
        }
    }
    while (_l_exit);
    _m_current_line_idx = 0;
    _m_current_line     = u8"";
    return false;
}

_END_ABC_UTILITY_IO_NS