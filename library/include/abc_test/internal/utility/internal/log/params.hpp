#pragma once
#include "abc_test/internal/utility/internal/log/enum.hpp"

_BEGIN_ABC_UTILITY_INTERNAL_NS
	struct internal_log_t;
	/*!
	* Paramter struct which describes the different options which can be used with the interna log.
	*/
	struct internal_log_params_t
	{
	public:
		/*!
		* Default constructor for internal_log_params
		*/
		__constexpr
			internal_log_params_t(
			) noexcept;
		/*!
		* Checks if the interna logger enum given as an argument is set.
		*/
		__constexpr
			bool
			is_set(
				const internal_log_enum_t _a_internal_looger_enum
			) const noexcept;
		friend class internal_log_t;
		bool _m_parsing_seed_on;
		bool _m_test_info_on;
		bool _m_repetition_info_on;
		bool _m_main_info_on;
	};
	_END_ABC_UTILITY_INTERNAL_NS

_BEGIN_ABC_UTILITY_INTERNAL_NS
	__constexpr_imp
		internal_log_params_t::internal_log_params_t(
		) noexcept
		: _m_parsing_seed_on(true)
		, _m_test_info_on(true)
		, _m_repetition_info_on(true)
		, _m_main_info_on(true)
	{

	}
	__constexpr_imp
		bool
		internal_log_params_t::is_set(
			const internal_log_enum_t _a_internal_logger_enum
		) const noexcept
	{
		using enum internal_log_enum_t;
		switch (_a_internal_logger_enum)
		{
		case NONE:
			return true;
		case PARSING_SEED:
			return _m_parsing_seed_on;
		case TEST_INFO:
			return _m_test_info_on;
		case REPETITION_INFO:
			return _m_repetition_info_on;
		case MAIN_INFO:
			return _m_main_info_on;
		default:
			return false;
		}
	}
	_END_ABC_UTILITY_INTERNAL_NS