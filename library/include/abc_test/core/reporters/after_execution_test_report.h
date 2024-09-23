#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/ds/test_data/invoked_test_data.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"
#include "abc_test/core/reporters/mid_execution_test_warning.h"
#include <source_location>

_BEGIN_ABC_REPORTERS_NS
struct after_execution_test_report_t
{
public:
	__constexpr
		after_execution_test_report_t(
		) noexcept;
	__constexpr
		after_execution_test_report_t(
			const ds::invoked_test_info_t& _a_iti,
			const test_options_t* _a_test_options
		) noexcept;
	__constexpr
		bool
		passed(
		) const noexcept;
	__constexpr
		void
		add_mid_execution_test_report(
			reporters::mid_execution_test_report_t* _a_metr
		) noexcept;
	__constexpr
		void
		add_mid_execution_warning(
			reporters::mid_execution_test_warning_t* _a_metr
		) noexcept;
	__constexpr
		void
		add_repetition_tree(
			const ds::repetition_tree_t& _a_repetition_tree,
			const test_options_t* _a_test_options
		) noexcept;
	__constexpr
		const mid_execution_test_reporters_t&
		reports(
		) const noexcept;
	__constexpr
		const std::optional<std::string>&
		name(
		) const noexcept;
	__constexpr
		const std::optional<std::source_location>&
		location(
		) const noexcept;
	__constexpr
		const std::string_view
		test_path(
		) const noexcept;
	__constexpr
		const std::string_view
		seed_used(
		) const noexcept;
	__constexpr
		const std::string_view
		seed_used_in_hex(
		) const noexcept;
	__constexpr
		const std::string_view
		seed_to_use_to_re_run_text(
		) const noexcept;
	__constexpr
		const std::string_view
		seed_to_use_to_re_run_text_in_hex(
		) const noexcept;
	__constexpr
		std::size_t
		test_warnings_recieved(
		) const noexcept;
	__constexpr
		std::size_t
		test_reports_recieved(
		) const noexcept;
	__constexpr
		std::size_t
		test_reports_passed(
		) const noexcept;
	__constexpr
		std::size_t
		test_reports_failed(
		) const noexcept;
	__constexpr
		bool
		terminated_early(
		) const noexcept;
private:
	mid_execution_test_reporters_t _m_mid_execution_reports;
	mid_execution_test_warnings_t _m_mid_execution_warnings;
	std::optional<std::string> _m_name;
	std::optional<std::source_location> _m_source_location;
	std::string _m_test_path;
	std::string _m_seed;
	std::string _m_seed_in_hex;
	std::string _m_seed_to_use_to_re_run_test;
	std::string _m_seed_to_use_to_re_run_test_in_hex;
	bool _m_passed;
	bool _m_terminated_early;
	std::size_t _m_total_test_reports_recieved;
	std::size_t _m_total_test_reports_passed;
	std::size_t _m_total_test_reports_failed;
	const test_options_t* _m_opts;
	//ds::repetition_tree_t _m_repetition_tree_if_repeating_test;
	__constexpr
		after_execution_test_report_t(
			const mid_execution_test_reporters_t& _a_mid_execution_reports,
			const std::optional<std::string>& _a_name,
			const std::optional<std::source_location>& _a_source_location,
			const std::string_view _a_test_path,
			const std::string_view _a_seed,
			const std::string_view _a_seed_in_hex,
			const bool _a_passed,
			const bool _a_terminated_early,
			const std::size_t _a_total_test_reports_recieved,
			const std::size_t _a_total_test_reports_passed,
			const std::size_t _a_total_test_reports_failed,
			const test_options_t* _a_test_options
		) noexcept;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	after_execution_test_report_t::after_execution_test_report_t(
	) noexcept
	: after_execution_test_report_t(
		mid_execution_test_reporters_t(),
		std::optional<std::string>(),
		std::optional<std::source_location>(),
		"",
		"","",false,false,0,0,0,nullptr
	)
{

}
__constexpr_imp
	after_execution_test_report_t::after_execution_test_report_t(
		const ds::invoked_test_info_t& _a_iti,
		const test_options_t* _a_test_options
	) noexcept
	: after_execution_test_report_t(
		mid_execution_test_reporters_t(),
		_a_iti.has_post_setup_test_data() && 
		_a_iti.post_setup_test_data().has_registered_test_data() ?
		std::optional<std::string>(_a_iti.post_setup_test_data().registered_test_data()._m_description) :
		std::optional<std::string>(),
		_a_iti.has_post_setup_test_data() &&
		_a_iti.post_setup_test_data().has_registered_test_data() ?
		_a_iti.post_setup_test_data().registered_test_data()._m_source_location :
		std::optional<std::source_location>(),
		_a_iti.has_post_setup_test_data() &&
		_a_iti.post_setup_test_data().has_registered_test_data() ?
		_a_iti.post_setup_test_data().registered_test_data()._m_test_path : "",
		_a_iti.has_post_setup_test_data() &&
		_a_iti.post_setup_test_data().has_repetition_data() ?
		_a_iti.post_setup_test_data().repetition_data().print_repetition_tree() : "",
		_a_iti.has_post_setup_test_data() &&
		_a_iti.post_setup_test_data().has_repetition_data() ?
		_a_iti.post_setup_test_data().repetition_data().print_repetition_tree(*_a_test_options) : "",
		true,false,0,0,0, _a_test_options
	)
{

}
__constexpr_imp
	bool
	after_execution_test_report_t::passed(
	) const noexcept
{
	return _m_passed;
}
__constexpr_imp
	void
	after_execution_test_report_t::add_mid_execution_test_report(
		reporters::mid_execution_test_report_t* _a_metr
) noexcept
{
	++_m_total_test_reports_recieved;
	if (_a_metr->passed())
	{
		++_m_total_test_reports_passed;
	}
	else
	{
		++_m_total_test_reports_failed;
		_m_passed = false;
	}
	if (_a_metr->early_termination() && not _a_metr->passed())
	{
		_m_terminated_early = true;
	}
	if (_m_opts->_m_store_passed_test_assertions || not _a_metr->passed())
	{
		_m_mid_execution_reports.push_back(mid_execution_test_report_ptr_t(_a_metr));
	}
}
__constexpr_imp
	void
	after_execution_test_report_t::add_mid_execution_warning(
		reporters::mid_execution_test_warning_t* _a_metr
	) noexcept
{
	_m_mid_execution_warnings.push_back(mid_execution_test_warning_ptr_t(_a_metr));
}
__constexpr_imp
	void
	after_execution_test_report_t::add_repetition_tree(
		const ds::repetition_tree_t& _a_repetition_tree,
		const test_options_t* _a_test_options
	) noexcept
{
	_m_seed_to_use_to_re_run_test = 
		_a_repetition_tree.print_repetition_tree();
	_m_seed_to_use_to_re_run_test_in_hex =
		_a_repetition_tree.print_repetition_tree(*_a_test_options);
}
__constexpr_imp
	const mid_execution_test_reporters_t&
	after_execution_test_report_t::reports(
	) const noexcept
{
	return _m_mid_execution_reports;
}
__constexpr_imp
	const std::optional<std::string>&
	after_execution_test_report_t::name(
	) const noexcept
{
	return _m_name;
}
__constexpr_imp
	const std::optional<std::source_location>&
	after_execution_test_report_t::location(
	) const noexcept
{
	return _m_source_location;
}
__constexpr_imp
	const std::string_view
	after_execution_test_report_t::test_path(
) const noexcept
{
	return _m_test_path;
}
__constexpr_imp
	const std::string_view
	after_execution_test_report_t::seed_used(
	) const noexcept
{
	return _m_seed;
}
__constexpr_imp
	const std::string_view
	after_execution_test_report_t::seed_used_in_hex(
	) const noexcept
{
	return _m_seed_in_hex;
}
__constexpr_imp
	const std::string_view
	after_execution_test_report_t::seed_to_use_to_re_run_text(
	) const noexcept
{
	return _m_seed_to_use_to_re_run_test;
}
__constexpr_imp
	const std::string_view
	after_execution_test_report_t::seed_to_use_to_re_run_text_in_hex(
	) const noexcept
{
	return _m_seed_to_use_to_re_run_test_in_hex;
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::test_warnings_recieved(
) const noexcept
{
	return _m_mid_execution_warnings.size();
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::test_reports_recieved(
	) const noexcept
{
	return _m_total_test_reports_recieved;
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::test_reports_passed(
	) const noexcept
{
	return _m_total_test_reports_passed;
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::test_reports_failed(
) const noexcept
{
	return _m_total_test_reports_failed;
}
__constexpr_imp
	bool
	after_execution_test_report_t::terminated_early(
) const noexcept
{
	return _m_terminated_early;
}
__constexpr_imp
	after_execution_test_report_t::after_execution_test_report_t(
		const mid_execution_test_reporters_t& _a_mid_execution_reports,
		const std::optional<std::string>& _a_name,
		const std::optional<std::source_location>& _a_source_location,
		const std::string_view _a_test_path,
		const std::string_view _a_seed,
		const std::string_view _a_seed_in_hex,
		const bool _a_passed,
		const bool _a_terminated_early,
		const std::size_t _a_total_test_reports_recieved,
		const std::size_t _a_total_test_reports_passed,
		const std::size_t _a_total_test_reports_failed,
		const test_options_t* _a_opts
	) noexcept
	: _m_mid_execution_reports(_a_mid_execution_reports)
	, _m_name(_a_name)
	, _m_source_location(_a_source_location)
	, _m_test_path(_a_test_path)
	, _m_seed(_a_seed)
	, _m_seed_in_hex(_a_seed_in_hex)
	, _m_passed(_a_passed)
	, _m_terminated_early(_a_terminated_early)
	, _m_total_test_reports_recieved(_a_total_test_reports_recieved)
	, _m_total_test_reports_passed(_a_total_test_reports_passed)
	, _m_total_test_reports_failed(_a_total_test_reports_failed)
	, _m_opts(_a_opts)

{

}
_END_ABC_REPORTERS_NS