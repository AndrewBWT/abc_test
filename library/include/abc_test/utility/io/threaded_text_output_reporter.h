#pragma once

#include "abc_test/utility/io/threaded_ostream_output_reporter.h"
#include "abc_test/utility/io/file/file_name.h"


_BEGIN_ABC_UTILITY_IO_NS
	struct threated_text_output_reporter_t :
		public threaded_ostream_output_reporter_t
	{
	public:
		__no_constexpr
			threated_text_output_reporter_t(
			) noexcept = delete;
		__no_constexpr
			~threated_text_output_reporter_t(
			) noexcept;
		__no_constexpr
			threated_text_output_reporter_t(
				const file_name_t& _a_file_name
			) noexcept;
		__no_constexpr
			threated_text_output_reporter_t(
				std::ostream& _a_ostream
			) noexcept;
	protected:
		std::optional<file_name_t> _m_file_name;
		std::size_t _m_line_len;
		__no_constexpr
			threated_text_output_reporter_t(
				const std::optional<file_name_t>& _a_file_name,
				std::ostream& _a_ostream
			) noexcept;
	};
_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
	__no_constexpr_imp
		threated_text_output_reporter_t::~threated_text_output_reporter_t(
		) noexcept
	{
		if (_m_file_name.has_value())
		{
			delete this->_m_output_stream.get_wrapped();
		}
	}
__no_constexpr_imp
		threated_text_output_reporter_t::threated_text_output_reporter_t(
			const file_name_t& _a_file_name
		) noexcept
		: threated_text_output_reporter_t(
			std::optional<file_name_t>(_a_file_name),
			*new std::ofstream(_a_file_name.file_path()))
	{

	}
	__no_constexpr_imp
		threated_text_output_reporter_t::threated_text_output_reporter_t(
			std::ostream& _a_ostream
		) noexcept
		: threated_text_output_reporter_t(
			std::optional<file_name_t>(), _a_ostream)
	{

	}
	__no_constexpr_imp
		threated_text_output_reporter_t::threated_text_output_reporter_t(
			const std::optional<file_name_t>& _a_file_name,
			std::ostream& _a_ostream
		) noexcept
		: threaded_ostream_output_reporter_t(_a_ostream)
		, _m_file_name(_a_file_name)
	{

	}
	_END_ABC_UTILITY_IO_NS