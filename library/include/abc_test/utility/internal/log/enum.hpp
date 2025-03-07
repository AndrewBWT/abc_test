#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include <string>

_BEGIN_ABC_UTILITY_INTERNAL_NS
	/*!
	* Enum used by logger to categorise a message.
	*/
	enum class internal_log_enum_t
	{
		NONE,
		PARSING_SEED,
		TEST_INFO,
		REPETITION_INFO,
		MAIN_INFO,
		THREAD_MAPPING
	};
	/*!
	* Converts the given enum to a string representation.
	*/
	__constexpr
		std::string
		to_str(
			const internal_log_enum_t _a_internal_logger_enum
		) noexcept;
	_END_ABC_UTILITY_INTERNAL_NS

_BEGIN_ABC_UTILITY_INTERNAL_NS
	__constexpr_imp
		std::string
		to_str(
			const internal_log_enum_t _a_internal_logger_enum
		) noexcept
	{
		using enum internal_log_enum_t;
		switch (_a_internal_logger_enum)
		{
		case NONE:
			return "NONE";
		case PARSING_SEED:
			return "PARSING_SEED";
		case TEST_INFO:
			return "TEST_INFO";
		case REPETITION_INFO:
			return "REPETITION_INFO";
		case MAIN_INFO:
			return "MAIN_INFO";
		case THREAD_MAPPING:
			return "THREAD_MAPPING";
		default:
			return "UNKNOWN";
		}
	}
	_END_ABC_UTILITY_INTERNAL_NS