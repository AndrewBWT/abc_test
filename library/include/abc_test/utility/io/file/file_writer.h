#pragma once

#include "abc_test/utility/io/file/file_name.h"

_BEGIN_ABC_UTILITY_IO_NS
	struct file_line_writer_t
	{
	public:
		__constexpr
			file_line_writer_t(
				const file_name_with_extension_t& _a_file_name,
				const test_options_t* _a_test_options
			);
		__constexpr
			file_line_writer_t(
			) noexcept;
		__constexpr
			void
			write_comment(
				const std::string_view _a_str_to_write
			) noexcept;
		__constexpr
			void
			write_line(
				const std::string_view _a_str_to_write
			) noexcept;
	private:
		size_t _m_current_line_idx;
		std::unique_ptr<std::ofstream> _m_file_handler;
		file_name_with_extension_t _m_file_name;
		const test_options_t* _m_test_options;
	};
_END_ABC_NS
_END_NS

_BEGIN_ABC_UTILITY_IO_NS
	__constexpr_imp
		file_line_writer_t::file_line_writer_t(
			const file_name_with_extension_t& _a_file_name,
			const test_options_t* _a_test_options
		) 
		: _m_file_handler(std::unique_ptr<std::ofstream>(nullptr))
		, _m_current_line_idx(0)
		, _m_file_name(_a_file_name)
		, _m_test_options(_a_test_options)
	{
		using namespace std;
		using namespace std::filesystem;
		using namespace errors;
		if (_m_file_name.is_valid())
		{
			const path _l_file_path{ _m_file_name.file_path() };
			_m_file_handler = std::unique_ptr<ofstream>(new ofstream(_l_file_path));
			if (not _m_file_handler.get()->is_open())
			{
				throw test_library_exception_t(
					fmt::format("Unable to open file_line_writer_t object as file \"{0}\", "
						"even though all checks suggest we should be able to.",
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
	__constexpr_imp
		file_line_writer_t::file_line_writer_t(
		) noexcept
		: _m_file_handler(std::unique_ptr<std::ofstream>(nullptr))
		, _m_current_line_idx(0)
		, _m_file_name(file_name_with_extension_t{})
		, _m_test_options(nullptr)
	{

	}
	__constexpr_imp
		void
		file_line_writer_t::write_comment(
			const std::string_view _a_str_to_write
		) noexcept
	{
		write_line(fmt::format("{0}{1}", 
			_m_test_options == nullptr ? "" : _m_test_options->_m_comment_str, 
			_a_str_to_write));
	}
	__constexpr_imp
		void
		file_line_writer_t::write_line(
			const std::string_view _a_str_to_write
		) noexcept
	{
		using namespace std;
		ofstream& _l_out{ *_m_file_handler.get() };
		_l_out << _a_str_to_write << std::endl;
	}
_END_ABC_NS
_END_NS