#pragma once

#include "abc_test/core/test_runner.h"
#include "abc_test/global.h"

#include <exception>


#define _INTERNAL_LOG(string_to_log,delete_after_use) abc::log_test_msg_t NAMED_COUNTER(log_var)(string_to_log,delete_after_use, std::source_location::current())
#define _TLOG(string_to_log) _INTERNAL_LOG(string_to_log,false)
#define _TLOG_(string_to_log) _INTERNAL_LOG(string_to_log,true)
#define _INTERNAL_VLOG(variable,delete_after_use) _INTERNAL_LOG(fmt::format("{0} = {1}", #variable,variable),delete_after_use)
#define _TVLOG(variable) _INTERNAL_VLOG(variable,false)
#define _TVLOG_(variable) _INTERNAL_VLOG(variable,true)


_BEGIN_ABC_NS
	struct log_test_msg_t
	{
	public:
		inline
			log_test_msg_t(
				const std::string_view _a_str,
				const bool _a_delete_after_use,
				const std::source_location& _a_source_location
			) noexcept
			: _m_str(_a_str)
			, _m_iterator(global::get_this_threads_test_runner_ref().add_error_info(this))
			, _m_delete_after_use(_a_delete_after_use)
		{
			global::get_this_threads_test_runner_ref().register_tests_most_recent_source(_a_source_location);
		}
		inline
			virtual
			~log_test_msg_t(
			) noexcept
		{
			if (std::uncaught_exceptions() > 0)
			{
				global::get_this_threads_test_runner_ref().add_cached_exception(_m_str);
			}
			if (not _m_delete_after_use)
			{
				global::get_this_threads_test_runner_ref().remove_error_info(_m_iterator);
			}
		}
		constexpr
			const std::string_view
			str(
			) const noexcept
		{
			return _m_str;
		}
		constexpr
			bool
			delete_after_use(
			) const noexcept
		{
			return _m_delete_after_use;
		}
	protected:
		std::string _m_str;
		std::list<const log_test_msg_t*>::iterator _m_iterator;
		bool _m_delete_after_use;
	};
_END_ABC_NS