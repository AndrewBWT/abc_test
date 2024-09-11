#pragma once

#include "abc_test/utility/io/threaded_ostream_output_reporter.h"
#include "abc_test/utility/io/file/file_name.h"


_BEGIN_ABC_UTILITY_IO_NS
	struct threated_text_output_reporter_t :
		public threaded_ostream_output_reporter_t
	{
	public:
		__constexpr
			threated_text_output_reporter_t(
			) noexcept = delete;
		__constexpr
			~threated_text_output_reporter_t(
			) noexcept;
		__constexpr
			threated_text_output_reporter_t(
				const file_name_t& _a_file_name
			) noexcept;
		__constexpr
			threated_text_output_reporter_t(
				std::ostream& _a_ostream
			) noexcept;
	protected:
		std::optional<file_name_t> _m_file_name;
		__constexpr
			threated_text_output_reporter_t(
				const std::optional<file_name_t>& _a_file_name,
				std::ostream& _a_ostream
			) noexcept;
	};
_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
	__constexpr_imp
		threated_text_output_reporter_t::~threated_text_output_reporter_t(
		) noexcept
	{
		if (_m_file_name.has_value())
		{
			delete this->_m_output_stream.get_wrapped();
		}
	}
	__constexpr_imp
		threated_text_output_reporter_t::threated_text_output_reporter_t(
			const file_name_t& _a_file_name
		) noexcept
		: threated_text_output_reporter_t(
			std::optional<file_name_t>(_a_file_name),
			*new std::ofstream(_a_file_name.file_path()))
	{

	}
	__constexpr
		threated_text_output_reporter_t::threated_text_output_reporter_t(
			std::ostream& _a_ostream
		) noexcept
		: threated_text_output_reporter_t(
			std::optional<file_name_t>(), _a_ostream)
	{

	}
	__constexpr_imp
		threated_text_output_reporter_t::threated_text_output_reporter_t(
			const std::optional<file_name_t>& _a_file_name,
			std::ostream& _a_ostream
		) noexcept
		: threaded_ostream_output_reporter_t(_a_ostream)
		, _m_file_name(_a_file_name)
	{

	}
	_END_ABC_UTILITY_IO_NS