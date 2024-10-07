#pragma once

#include "abc_test/core/test_runner.h"
#include "abc_test/global.h"

#include <exception>
#include "abc_test/core/ds/source/single_source.h"

#include "abc_test/utility/internal/macros.h"

#define _INTERNAL_LOG_VAR_NAME _l_log_var
#define _INTERNAL_LOG(_a_string_to_log,_a_delete_after_use, _a_str_representation)\
	abc::log_test_msg_t NAMED_COUNTER(_INTERNAL_LOG_VAR_NAME)(\
		abc::reports::single_source_t(_a_str_representation,std::source_location::current()),\
		_a_string_to_log,\
		_a_delete_after_use)
#define _TLOG(_a_str_to_log) _INTERNAL_LOG(_a_str_to_log,false,abc::utility::str::create_string({"_TLOG(",#_a_str_to_log,")"}))
#define _TLOG_(_a_str_to_log) _INTERNAL_LOG(_a_str_to_log,true,abc::utility::str::create_string({"_TLOG_(",#_a_str_to_log,")"}))
#define _INTERNAL_VLOG(_a_variable,_a_delete_after_use) \
		_INTERNAL_LOG(\
			fmt::format("{0} = {1}", #_a_variable,_a_variable),\
		_a_delete_after_use\
		)
#define _TVLOG(_a_var) _INTERNAL_VLOG(_a_var,false,abc::utility::str::create_string({"_TVLOG(",#_a_var,")"}))
#define _TVLOG_(_a_var) _INTERNAL_VLOG(_a_var,true,abc::utility::str::create_string({"_TVLOG_(",#_a_var,")"}))
		
//Logs are going to be able to be attached to an object!

_BEGIN_ABC_NS
struct log_test_msg_t
{
public:
	__no_constexpr
		log_test_msg_t(
			const reports::single_source_t& _a_single_source,
			const std::string_view _a_str,
			const bool _a_delete_after_use
		) noexcept;
	__no_constexpr
		~log_test_msg_t(
		) noexcept;
	__constexpr
		const std::string_view
		str(
		) const noexcept;
	__constexpr
		bool
		delete_after_use(
		) const noexcept;
protected:
	std::string _m_str;
	std::list<const log_test_msg_t*>::iterator _m_iterator;
	bool _m_delete_after_use;
};
using log_infos_t = std::vector<std::string>;
_END_ABC_NS

_BEGIN_ABC_NS

__no_constexpr_imp
	log_test_msg_t::log_test_msg_t(
		const reports::single_source_t& _a_single_source,
		const std::string_view _a_str,
		const bool _a_delete_after_use
	) noexcept
	: _m_str(_a_str)
	, _m_iterator(global::get_this_threads_test_runner_ref().add_error_info(this))
	, _m_delete_after_use(_a_delete_after_use)
{
	using namespace global;
	get_this_threads_test_runner_ref().register_tests_most_recent_source(_a_single_source);
}
__no_constexpr_imp
	log_test_msg_t::~log_test_msg_t(
	) noexcept
{
	using namespace std;
	using namespace global;
	if (uncaught_exceptions() > 0)
	{
		get_this_threads_test_runner_ref().add_cached_exception(_m_str);
	}
	if (not _m_delete_after_use)
	{
		get_this_threads_test_runner_ref().remove_error_info(_m_iterator);
	}
}
__constexpr_imp
	const std::string_view
	log_test_msg_t::str(
	) const noexcept
{
	return _m_str;
}
__constexpr_imp
	bool
	log_test_msg_t::delete_after_use(
	) const noexcept
{
	return _m_delete_after_use;
}
_END_ABC_NS