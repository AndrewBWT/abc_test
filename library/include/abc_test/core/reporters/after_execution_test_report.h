#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/ds/test_data/invoked_test_data.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"
#include "abc_test/core/reporters/mid_execution_test_warning.h"
#include <source_location>
#include <memory>

#include "abc_test/core/test_reports/mid_test_invokation_report/user_initialised_report.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/generic_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/unexpected_report.h"

_BEGIN_ABC_REPORTERS_NS
enum class enum_termination_type_t
{
	NO_TERMINATION,
	ASSERTION_TERMINATION,
	UNEXPECTED_TERMINATION
};
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
		add_repetition_tree(
			const ds::for_loop_stack_trie_t& _a_repetition_tree,
			const test_options_t* _a_test_options
		) noexcept;
	__constexpr
		const std::optional<std::string>&
		name(
		) const noexcept;
	__constexpr
		const ds::single_source_t&
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
		reports::opt_unexpected_report_t
		unexpected_termination(
		) const noexcept;
	__constexpr
		std::size_t
		assertions_recieved(
		) const noexcept;
	__constexpr
		std::size_t
		assertions_passed(
		) const noexcept;
	__constexpr
		std::size_t
		assertions_failed(
		) const noexcept;
	__constexpr
		enum_termination_type_t
		terminated_type(
		) const noexcept;
	template<
		bool Single_Source,
		typename Assertion_Status
	>
	__constexpr
		void
		add_assertion(
			const reports::generic_assertion_t<Single_Source, Assertion_Status>* _a_gur
		) noexcept;
	__constexpr
		void
		set_unexpected_termination(
			const reports::unexpected_report_t<true>* _a_ur
		) noexcept;
	__constexpr
		void
		add_warning(
			const reports::unexpected_report_t<false>* _a_warning
		) noexcept;
	__constexpr
		const reports::generic_user_report_collection_t&
		assertions(
		) const noexcept;
private:
	reports::generic_user_report_collection_t _m_assertions;
	reports::opt_unexpected_report_t _m_termination_report;
	reports::unexpected_non_terminating_report_collection_t _m_warnings;
//	generic_assertions _m_function_reports;
	std::optional<std::string> _m_name;
	ds::single_source_t _m_source_location;
	std::string _m_test_path;
	std::string _m_seed;
	std::string _m_seed_in_hex;
	std::string _m_seed_to_use_to_re_run_test;
	std::string _m_seed_to_use_to_re_run_test_in_hex;
	bool _m_passed;
	enum_termination_type_t _m_termination_type;
	// pass and no termination -> ok
	// pass and terminate early -> ok
	// pasa and unexpected termination -> fail
	// fail and no termination -> fail
	// fail and terminate early -> fail
	// fail and unexpected termination -> fail
	std::size_t _m_total_test_reports_recieved;
	std::size_t _m_total_test_reports_passed;
	std::size_t _m_total_test_reports_failed;
	const test_options_t* _m_opts;
	__constexpr
		after_execution_test_report_t(
		//	const mid_execution_test_reporters_t& _a_mid_execution_reports,
			const std::optional<std::string>& _a_name,
			const ds::single_source_t& _a_sl,
			const std::string_view _a_test_path,
			const std::string_view _a_seed,
			const std::string_view _a_seed_in_hex,
			const bool _a_passed,
			const enum_termination_type_t _a_termination_type,
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
	//	mid_execution_test_reporters_t(),
		std::optional<std::string>(),
		ds::single_source_t(),
		"",
		"","",false,enum_termination_type_t::NO_TERMINATION,0,0,0,nullptr
	)
{

}
__constexpr_imp
	after_execution_test_report_t::after_execution_test_report_t(
		const ds::invoked_test_info_t& _a_iti,
		const test_options_t* _a_test_options
	) noexcept
	: after_execution_test_report_t(
	//	mid_execution_test_reporters_t(),
		true && true ?
		std::optional<std::string>(_a_iti.post_setup_test_data().registered_test_data()._m_user_data.name) :
		std::optional<std::string>(),
		true && true ?
		_a_iti.post_setup_test_data().registered_test_data()._m_source :
		ds::single_source_t(),
		true && true ?
		_a_iti.post_setup_test_data().registered_test_data()._m_user_data.path : "",
		true &&
		_a_iti.post_setup_test_data().has_repetition_data() ?
		_a_iti.post_setup_test_data().repetition_data().print_for_loop_stack_trie() : "",
		true &&
		_a_iti.post_setup_test_data().has_repetition_data() ?
		_a_iti.post_setup_test_data().repetition_data().print_for_loop_stack_trie_compressed() : "",
		true,enum_termination_type_t::NO_TERMINATION,0,0,0, _a_test_options
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
	after_execution_test_report_t::add_repetition_tree(
		const ds::for_loop_stack_trie_t& _a_repetition_tree,
		const test_options_t* _a_test_options
	) noexcept
{
	_m_seed_to_use_to_re_run_test = 
		_a_repetition_tree.print_for_loop_stack_trie();
	_m_seed_to_use_to_re_run_test_in_hex =
		_a_repetition_tree.print_for_loop_stack_trie_compressed();
}
__constexpr_imp
	const std::optional<std::string>&
	after_execution_test_report_t::name(
	) const noexcept
{
	return _m_name;
}
__constexpr_imp
	const ds::single_source_t&
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
	return 0;
}
__constexpr_imp
reports::opt_unexpected_report_t
after_execution_test_report_t::unexpected_termination(
) const noexcept
{
	return _m_termination_report;
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::assertions_recieved(
	) const noexcept
{
	return _m_total_test_reports_recieved;
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::assertions_passed(
	) const noexcept
{
	return _m_total_test_reports_passed;
}
__constexpr_imp
	std::size_t
	after_execution_test_report_t::assertions_failed(
) const noexcept
{
	return _m_total_test_reports_failed;
}
__constexpr_imp
enum_termination_type_t
	after_execution_test_report_t::terminated_type(
) const noexcept
{
	return _m_termination_type;
}
template<
	bool Single_Source,
	typename Assertion_Status
>
__constexpr_imp
void
after_execution_test_report_t::add_assertion(
	const reports::generic_assertion_t<Single_Source, Assertion_Status>* _a_gur
) noexcept
{
	using namespace reports;
	_m_total_test_reports_recieved++;
	if (_a_gur->get_pass_status())
	{
		_m_total_test_reports_passed++;
	}
	else
	{
		_m_passed = false;
		_m_total_test_reports_failed++;
		if (_a_gur->terminated())
		{
			_m_termination_type = enum_termination_type_t::ASSERTION_TERMINATION;
		}
	}
	_m_assertions.push_back(generic_user_report_ptr_t(_a_gur));
}
__constexpr_imp
void
after_execution_test_report_t::set_unexpected_termination(
	const reports::unexpected_report_t<true>* _a_ur
) noexcept
{
	using namespace reports;
	_m_termination_report = opt_unexpected_report_t(_a_ur);
	_m_termination_type = enum_termination_type_t::UNEXPECTED_TERMINATION;
	_m_passed = false;
}
__constexpr_imp
void
after_execution_test_report_t::add_warning(
	const reports::unexpected_report_t<false>* _a_warning
) noexcept
{
	using namespace reports;
	_m_warnings.push_back(unexpected_non_terminating_report_ptr_t(_a_warning));
}
__constexpr_imp
const reports::generic_user_report_collection_t&
after_execution_test_report_t::assertions(
) const noexcept
{
	return _m_assertions;
}
__constexpr_imp
	after_execution_test_report_t::after_execution_test_report_t(
	//	const mid_execution_test_reporters_t& _a_mid_execution_reports,
		const std::optional<std::string>& _a_name,
		const ds::single_source_t& _a_sl,
		const std::string_view _a_test_path,
		const std::string_view _a_seed,
		const std::string_view _a_seed_in_hex,
		const bool _a_passed,
		const enum_termination_type_t _a_termination_type,
		const std::size_t _a_total_test_reports_recieved,
		const std::size_t _a_total_test_reports_passed,
		const std::size_t _a_total_test_reports_failed,
		const test_options_t* _a_opts
	) noexcept
	//: _m_mid_execution_reports(_a_mid_execution_reports)
	: _m_name(_a_name)
	, _m_source_location(_a_sl)
	, _m_test_path(_a_test_path)
	, _m_seed(_a_seed)
	, _m_seed_in_hex(_a_seed_in_hex)
	, _m_passed(_a_passed)
	, _m_termination_type(_a_termination_type)
	, _m_total_test_reports_recieved(_a_total_test_reports_recieved)
	, _m_total_test_reports_passed(_a_total_test_reports_passed)
	, _m_total_test_reports_failed(_a_total_test_reports_failed)
	, _m_opts(_a_opts)

{

}
_END_ABC_REPORTERS_NS