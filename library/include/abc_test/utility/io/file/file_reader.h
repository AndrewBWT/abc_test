#pragma once
#include <fstream>
#include <string>
#include "abc_test/utility/io/file/file_name_with_extension.h"
#include "abc_test/core/test_options.h"

#include <iostream>
#include <fmt/std.h>

_BEGIN_ABC_UTILITY_IO_NS
	struct file_line_reader_t
	{
	public:
		file_line_reader_t(
				const file_name_with_extension_t& _a_file_name,
				const test_options_t* _a_test_options
			);
		file_line_reader_t(
			) noexcept;
		constexpr
			const std::string_view
			current_line(
			) noexcept;
		constexpr
			bool
			has_current_line(
			) const noexcept;
		bool
			get_next_line(
			);
	private:
		size_t _m_current_line_idx;
		std::string _m_current_line;
		std::shared_ptr<std::ifstream> _m_file_handler;
		file_name_with_extension_t _m_file_name;
		const test_options_t* _m_test_options;
	};
_END_ABC_NS
_END_NS

_BEGIN_ABC_UTILITY_IO_NS
	inline 
		file_line_reader_t::file_line_reader_t(
			const file_name_with_extension_t& _a_file_name,
			const test_options_t* _a_test_options
		)
		: _m_current_line_idx(0)
		, _m_current_line("")
		, _m_file_handler(std::unique_ptr<std::ifstream>())
		, _m_file_name(_a_file_name)
		, _m_test_options(_a_test_options)
	{
		using namespace std;
		using namespace std::filesystem;
		using namespace errors;
		if (_m_file_name.is_valid())
		{
			const path _l_file_path{ _m_file_name.file_path() };
			if (not _m_file_name.exists())
			{
				throw test_library_exception_t(
					fmt::format("Unable to open file_line_reader_t object as file \"{0}\" does not exist",
						_m_file_name.file_path()));
			}
			_m_file_handler = std::shared_ptr<ifstream>(new ifstream(_l_file_path));
			if (_m_file_handler.get()->is_open())
			{
				get_next_line();
			}
			else
			{
				throw test_library_exception_t(
					fmt::format("Unable to open file_line_reader_t object as file \"{0}\", even though file exists.",
						_m_file_name.file_path()));
			}
		}
		else
		{
			throw test_library_exception_t(
				fmt::format("file name \"{0}\" has been flagged as being invalid.",
					_m_file_name.file_path()));
		}
	}
	inline
		file_line_reader_t::file_line_reader_t(
		) noexcept
		: _m_current_line_idx(0)
		, _m_current_line("")
		, _m_file_handler(std::unique_ptr<std::ifstream>())
		, _m_file_name(file_name_with_extension_t{})
		, _m_test_options(nullptr)
	{

	}
	inline constexpr
		const std::string_view
		file_line_reader_t::current_line(
		) noexcept
	{
		return _m_current_line;
	}
	inline constexpr
		bool
		file_line_reader_t::has_current_line(
		) const noexcept
	{
		return (_m_current_line_idx > 0);
	}
	inline 
		bool
		file_line_reader_t::get_next_line(
		) 
	{
		using namespace errors;
		using namespace std;
		ifstream& _l_file_hander{ *_m_file_handler.get() };
		do
		{
			++_m_current_line_idx;
			std::getline(_l_file_hander, _m_current_line);
			const bool _l_error_reading_file{ _l_file_hander.fail() || _l_file_hander.bad() };
			if (_l_file_hander.eof())
			{
				return false;
			}
			else if (_l_error_reading_file)
			{
				_m_current_line = "";
				_m_current_line_idx = 0;
				throw test_library_exception_t(
					fmt::format("Error encountered reading line {0} in file \"{1}\". "
						"The fail bit of the file was set to {2} and the bad_bit of the file was set to {3}",
						_m_current_line_idx,
						_m_file_name,
						_l_file_hander.fail(),
						_l_file_hander.bad()));
			}
			else if (_m_test_options != nullptr &&
				_m_current_line.starts_with(_m_test_options->_m_comment_str))
			{
				continue;
			}
			else
			{
				return true;
			}
		} while (true);
		return false;

	}
_END_ABC_NS
_END_NS