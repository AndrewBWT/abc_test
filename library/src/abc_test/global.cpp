#include "ab_test/global.h"

/*#include "ab_test/utility/internal/logging.h"

namespace ab::test::global
{
	__no_constexpr_or_inline_imp
		void
		log(
			const utility::internal_logger_enum_t _a_internal_logger_enum,
			const std::string_view _a_str
		) noexcept
	{
		using namespace utility;
		internal_logger_t& _l_il{ get_internal_library_logger() };
		_l_il.write(_a_internal_logger_enum, _a_str);
	}
	__no_constexpr_or_inline_imp
		void
		set_internal_library_logger(
			const utility::internal_logger_params_t& _a_internal_looger_params
		) noexcept
	{
		using namespace utility;
		internal_logger_t& _l_il{ get_internal_library_logger() };
		_l_il.set_internal_logger_params(_a_internal_looger_params);
	}
	namespace
	{
		__no_constexpr_or_inline_imp
			utility::internal_logger_t&
			get_internal_library_logger(
			) noexcept
		{
			using namespace utility;
			static internal_logger_t _s_il;
			return _s_il;
		}
	}
}*/