#pragma once
#include <vector>
#include "abc_test/core/test_reporter.h"
#include <optional>
#include <source_location>

namespace ab::test
{
	struct pc_test_reporter : public test_reporter
	{
	public:
		virtual
			constexpr
			void
			entering_test(
				const test_info_t& _a_test_info
			) noexcept;
		virtual
			constexpr
			bool
			process_error(
				const error_info_t& _a_error_info
			) noexcept;
		constexpr
			const std::vector<error_info_t>&
			get_error_info(
			) const noexcept;
		constexpr
			void
			clear_errors(
			) noexcept;
		constexpr
			size_t
			tests_ran(
			) const noexcept;
		constexpr
			const std::vector<test_info_t>&
			get_all_test_infos(
			) const noexcept;
	private:
		std::vector<error_info_t> _m_error_infos;
		size_t _m_tests_ran;
		std::vector<test_info_t> _m_all_test_info;
	};
}

namespace ab::test
{
	inline
		constexpr
		void
		pc_test_reporter::entering_test(
			const test_info_t& _a_test_info
		) noexcept
	{
		_m_all_test_info.push_back(_a_test_info);
	}
	inline
		constexpr
		bool
		pc_test_reporter::process_error(
			const error_info_t& _a_error_info
		) noexcept
	{
		_m_error_infos.push_back(_a_error_info);
		return true;
	}
	constexpr
		const std::vector<error_info_t>&
		pc_test_reporter::get_error_info(
		) const noexcept
	{
		return _m_error_infos;
	}
	constexpr
		void
		pc_test_reporter::clear_errors(
		) noexcept
	{
		_m_error_infos.clear();
	}
	inline constexpr
		size_t
		pc_test_reporter::tests_ran(
		) const noexcept
	{
		return _m_tests_ran;
	}
	constexpr
		const std::vector<test_info_t>&
		pc_test_reporter::get_all_test_infos(
		) const noexcept
	{
		return _m_all_test_info;
	}
}