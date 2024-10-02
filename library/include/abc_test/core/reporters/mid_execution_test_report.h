#pragma once
/*
#include "abc_test/utility/internal/macros.h"
#include <vector>
#include <string>

_BEGIN_ABC_REPORTERS_NS
using str_collection_t = std::vector<std::string>;
class mid_execution_test_report_t
{
public:
	__constexpr
		mid_execution_test_report_t(
			const bool _a_passed,
			const std::source_location& _a_source_location,
			const bool _a_early_termination,
			const str_collection_t& _a_log_infos
		) noexcept;
	__constexpr
		bool
		passed(
		) const noexcept;
	__constexpr
		const std::source_location&
		source_location(
		) const noexcept;
	__constexpr
		bool
		early_termination(
		) const noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept = 0;
	__constexpr
		const str_collection_t&
		log_infos(
		) const noexcept;
protected:
	bool _m_passed;
	std::source_location _m_source_location;
	bool _m_early_termination;
	str_collection_t _m_log_infos;
	__constexpr
		std::string
		unformatted_base_string(
		) const noexcept;
};
using mid_execution_test_report_ptr_t = std::shared_ptr<mid_execution_test_report_t>;
using mid_execution_test_reporters_t = std::vector<mid_execution_test_report_ptr_t>;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	mid_execution_test_report_t::mid_execution_test_report_t(
		const bool _a_passed,
		const std::source_location& _a_source_location,
		const bool _a_early_termination,
		const str_collection_t& _a_log_infos
	) noexcept
	: _m_passed(_a_passed)
	, _m_source_location(_a_source_location)
	, _m_early_termination(_a_early_termination)
	, _m_log_infos(_a_log_infos)
{

}
__constexpr_imp
	bool
	mid_execution_test_report_t::passed(
	) const noexcept
{
	return _m_passed;
}
__constexpr_imp
	const std::source_location&
	mid_execution_test_report_t::source_location(
	) const noexcept
{
	return _m_source_location;
}
__constexpr_imp
	bool
	mid_execution_test_report_t::early_termination(
	) const noexcept
{
	return _m_early_termination;
}
__constexpr_imp
	const str_collection_t&
	mid_execution_test_report_t::log_infos(
) const noexcept
{
	return _m_log_infos;
}
__constexpr_imp
	std::string
	mid_execution_test_report_t::unformatted_base_string(
) const noexcept
{
	return fmt::format(
		"{0} = {1},"
		"{2} = {3},"
		"{4} = {5},"
		"{6} = {7},",
		"_m_passed", _m_passed,
		"_m_source_location", _m_source_location,
		"_m_early_termination", _m_early_termination,
		"_m_log_infos", _m_log_infos
	);
}
_END_ABC_REPORTERS_NS*/