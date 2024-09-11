#pragma once

#include "abc_test/core/errors/test_failure_info.h"

_BEGIN_ABC_ERRORS_NS
	using const_test_failures_info_itt_t = std::vector<test_failure_info_t>::const_iterator;
	struct test_failures_info_t
	{
	public:
		__constexpr
			bool
			passed(
			) const noexcept;
		__constexpr
			const_test_failures_info_itt_t
			begin(
			) const noexcept;
		__constexpr
			const_test_failures_info_itt_t
			end(
			) const noexcept;
		__no_constexpr
			void
			add_error_msg(
				const test_failure_info_t& _a_error_info
			) noexcept;
		__constexpr
			void
			reset(
			) noexcept;
		__constexpr
			size_t
			size(
			) const noexcept;
	private:
		std::vector<test_failure_info_t> _m_test_failures;
	};
	_END_ABC_NS
		_END_NS

		_BEGIN_ABC_ERRORS_NS
	__constexpr_imp
		bool
		test_failures_info_t::passed(
		) const noexcept
	{
		return _m_test_failures.size() == 0;
	}
	__constexpr_imp
		const_test_failures_info_itt_t
		test_failures_info_t::begin(
		) const noexcept
	{
		return _m_test_failures.begin();
	}
	__constexpr_imp
		const_test_failures_info_itt_t
		test_failures_info_t::end(
		) const noexcept
	{
		return _m_test_failures.end();
	}
	__no_constexpr_imp
		void
		test_failures_info_t::add_error_msg(
			const test_failure_info_t& _a_error_info
		) noexcept
	{
		_m_test_failures.push_back(_a_error_info);
	}
	__constexpr_imp
		void
		test_failures_info_t::reset(
		) noexcept
	{
		_m_test_failures.clear();
	}
	__constexpr_imp
		size_t
		test_failures_info_t::size(
		) const noexcept
	{
		return _m_test_failures.size();
	}
	_END_ABC_NS
		_END_NS